// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <mm/config/config.h>

#include <mm/slab_arena.h>
#include <mm/alloc.h>
#include <mm/slab.h>

/* --------------------------------------------------------------------------
 * INTERNAL TYPES
 * -------------------------------------------------------------------------- */

struct _mm_slab_arena {
	struct mm_slab **pool;
	size_t count;
};

/* --------------------------------------------------------------------------
 * LOCAL VARIABLES
 * -------------------------------------------------------------------------- */

static struct _mm_slab_arena _slab_arena = { 0 };

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

int mm_slab_arena_create(struct mm_slab_arena_config *config, size_t count)
{
	int i;

	if (!config || !count)
		return -EINVAL;

	_slab_arena.count = count;
	_slab_arena.pool = mm_calloc(count, sizeof(struct sys_slab *));
	if (!_slab_arena.pool)
		return -ENOMEM;

	for (i = 0; i < count; i++) {
		if (i && (config[i].esize < config[i - 1].esize))
			return i - 1;

		_slab_arena.pool[i] = mm_slab_create(NULL, MM_ALIGN, config[i].esize, config[i].ecount);
	}

	return 0;
}

int mm_slab_arena_destroy(void)
{
	int i;
	bool destroyed = true;

	for (i = 0; i < _slab_arena.count; i++) {
		int err;

		if (!_slab_arena.pool[i])
			continue;

		err = mm_slab_destroy(_slab_arena.pool[i]);
		if (err < 0)
			continue;

		destroyed = destroyed && (err == 0);
		_slab_arena.pool[i] = NULL;
	}

	if (destroyed) {
		mm_free(_slab_arena.pool);
		_slab_arena.pool = NULL;
		_slab_arena.count = 0;
		return 0;
	}

	return -EAGAIN;
}

__attribute__((__malloc__(mm_slab_arena_free, 1)))
void *mm_slab_arena_malloc(size_t size)
{
	int i, err;
	void *ptr = NULL;

	if (!_slab_arena.pool || !size)
		return NULL;

	for (i = 0; i < _slab_arena.count; i++) {
		size_t esize;

		if (!_slab_arena.pool[i])
			continue;

		err = mm_slab_stats(_slab_arena.pool[i], &esize, NULL, NULL, NULL, NULL);
		if (err < 0)
			continue;

		if (size > esize)
			continue;

		ptr = mm_slab_alloc(_slab_arena.pool[i]);
		if (!ptr)
			break;

		return ptr;
	}

	ptr = mm_malloc(size);
	return ptr;
}

__attribute__((__malloc__(mm_slab_arena_free, 1)))
void *mm_slab_arena_calloc(size_t nmemb, size_t size)
{
	void *ptr;

	if (!nmemb || !size)
		return NULL;

	ptr = mm_slab_arena_malloc(nmemb * size);
	if (ptr)
		memset(ptr, 0x0, nmemb * size);

	return ptr;
}

int mm_slab_arena_free(void *ptr)
{
	int i;

	if (!_slab_arena.pool || !ptr)
		return -EINVAL;

	for (i = 0; i < _slab_arena.count; i++) {
		int err;

		if (!_slab_arena.pool[i])
			continue;

		err = mm_slab_free(_slab_arena.pool[i], ptr) ;
		if (err < 0)
			continue;

		return 0;
	}

	mm_free(ptr);

	return 0;
}

int mm_slab_arena_stats(struct mm_slab_arena_stats **stats, size_t *count)
{
	struct mm_slab_arena_stats *s;
	int i;

	if (!_slab_arena.pool || !stats || !count)
		return -EINVAL;

	s = mm_calloc(_slab_arena.count, sizeof(struct mm_slab_arena_stats));
	if (!s)
		return -ENOMEM;

	for (i = 0; i < _slab_arena.count; i++) {
		int err;

		err = mm_slab_stats(_slab_arena.pool[i], &s[i].esize, &s[i].ecount, &s[i].allocated, &s[i].missed, &s[i].freed);
		if (err < 0) {
			*count = 0;
			*stats = NULL;
			mm_free(s);
			return err;
		}
	}

	*count = _slab_arena.count;
	*stats = s;
	return *count;
}
