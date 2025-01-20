// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <freebsd/sys/sys/bitcount.h>
#include <freebsd/sys/sys/bitstring.h>

#include <mm/config/cdefs.h>
#include <mm/config/mutex.h>

#include <mm/alloc.h>
#include <mm/slab.h>

/* --------------------------------------------------------------------------
 * INTERNAL TYPES
 * -------------------------------------------------------------------------- */

/* SLAB in hexadecimal */
#define MM_SLAB_MAGIC 0x83766566

struct mm_slab {
	uint32_t magic;
	void *pool;
	void *pool_origin;
	size_t alignment;
	size_t esize;
	size_t ecount;
	MUTEX_TYPE lock;

	struct {
		size_t allocated;
		size_t missed;
		size_t freed;
	} stats;

	bitstr_t *allocated;
};

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

struct mm_slab *mm_slab_create(void *buffer, size_t alignment, size_t esize, size_t ecount)
{
	struct mm_slab *slab;
	int err;

	if (!esize || !ecount)
		return NULL;

	if (alignment && !ISPOWEROF2(alignment))
		return NULL;

	slab = mm_malloc(sizeof(struct mm_slab));
	slab->magic = MM_SLAB_MAGIC;

	err = MUTEX_INIT(slab->lock);
	if (err < 0) {
		mm_free(slab);
		return NULL;
	}

	slab->ecount = ecount;

	slab->allocated = bit_alloc(slab->ecount);
	if (!slab->allocated) {
		MUTEX_DESTROY(slab->lock);
		mm_free(slab);
		return NULL;
	}

	slab->pool = NULL;
	slab->pool_origin = NULL;
	slab->alignment = alignment;
	slab->esize = esize;
	slab->stats.allocated = 0;
	slab->stats.missed = 0;
	slab->stats.freed = 0;

	if (buffer) {
		slab->pool = buffer;

		return slab;
	}

	if (alignment)
		slab->esize = ROUNDUP(esize, alignment);

	slab->pool_origin = mm_malloc(ecount * slab->esize + alignment);
	if (!slab->pool_origin) {
		mm_free(slab->allocated);
		MUTEX_DESTROY(slab->lock);
		mm_free(slab);
		return NULL;
	}

	if (alignment)
		slab->pool = (void *)ROUNDUP(((uintptr_t)slab->pool_origin), alignment);
	else
		slab->pool = slab->pool_origin;

	return slab;
}

int mm_slab_destroy(struct mm_slab *slab)
{
	if (!slab)
		return -EINVAL;

	if (slab->magic != MM_SLAB_MAGIC)
		return -EIO;

	if (slab->allocated) {
		size_t count;

		bit_count(slab->allocated, 0, slab->ecount, &count);
		if (count)
			return -EAGAIN;

		mm_free(slab->allocated);
	}

	if (slab->pool_origin)
		mm_free(slab->pool_origin);

	MUTEX_DESTROY(slab->lock);
	mm_free(slab);

	return 0;
}

__attribute__((__malloc__(mm_slab_free, 2)))
void *mm_slab_alloc(struct mm_slab *slab)
{
	void *ptr;
	int idx;

	if (!slab)
		return NULL;

	if (slab->magic != MM_SLAB_MAGIC)
		return NULL;

	if (!slab->allocated)
		return NULL;

	MUTEX_LOCK(slab->lock);
	bit_ffc(slab->allocated, slab->ecount, &idx);
	if (idx <= slab->ecount) {
		ptr = (void *)((uintptr_t)slab->pool + idx * slab->esize);
		bit_set(slab->allocated, idx);
		slab->stats.allocated++;
	} else {
		ptr = NULL;
		slab->stats.missed++;
	}
	MUTEX_UNLOCK(slab->lock);

	return ptr;
}

int mm_slab_free(struct mm_slab *slab, void *ptr)
{
	size_t idx;

	if (!slab || !ptr)
		return -EINVAL;

	if (slab->magic != MM_SLAB_MAGIC)
		return -EIO;

	MUTEX_LOCK(slab->lock);
	if (((uintptr_t)ptr < (uintptr_t)slab->pool) || ((uintptr_t)ptr > (uintptr_t)slab->pool + slab->esize * slab->ecount)) {
		MUTEX_UNLOCK(slab->lock);
		return -ERANGE;
	}

	idx = ((uintptr_t)ptr - (uintptr_t)slab->pool) / slab->esize;

	if (slab->allocated)
		bit_clear(slab->allocated, idx);
	slab->stats.freed++;
	MUTEX_UNLOCK(slab->lock);

	return 0;
}

int mm_slab_stats(struct mm_slab *slab,
		   size_t *esize, size_t *ecount, size_t *allocated, size_t *missed, size_t *freed)
{
	if (!slab)
		return -EINVAL;

	if (slab->magic != MM_SLAB_MAGIC)
		return -EIO;

	MUTEX_LOCK(slab->lock);
	if (esize)
		*esize = slab->esize;
	if (ecount)
		*ecount = slab->ecount;
	if (allocated)
		*allocated = slab->stats.allocated;
	if (missed)
		*missed = slab->stats.missed;
	if (freed)
		*freed = slab->stats.freed;
	MUTEX_UNLOCK(slab->lock);

	return 0;
}
