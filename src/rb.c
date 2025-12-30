// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#include <mm/config/cdefs.h>

#include <mm/rb.h>

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

int rb_init(struct rb *rb, void *array, size_t esize, size_t ecount)
{
	int err;

	if (!rb || !array || !esize || !ecount)
		return -EINVAL;

	err = rbi_init(&rb->rbi, ecount);
	if (err < 0)
		return err;

	rb->array = array;
	rb->esize = esize;

	return 0;
}

bool rb_put(struct rb *rb, const void *element)
{
	uint8_t *dest;
	size_t pos;

	if (!rb)
		return false;

	if (rbi_is_full(&rb->rbi))
		return false;

	pos = rbi_put(&rb->rbi);
	dest = (uint8_t *)rb->array + pos * rb->esize;
	memcpy(dest, element, rb->esize);

	return true;
}

bool rb_get(struct rb *rb, void *element)
{
	uint8_t *src;
	size_t pos;

	if (!rb)
		return false;

	if (rbi_is_empty(&rb->rbi))
		return false;

	pos = rbi_get(&rb->rbi);
	src = (uint8_t *)rb->array + pos * rb->esize;
	memcpy(element, src, rb->esize);

	return true;
}

void *rb_peek(const struct rb *rb)
{
	size_t pos;

	if (!rb)
		return NULL;

	if (rbi_is_empty(&rb->rbi))
		return NULL;

	pos = rbi_peek(&rb->rbi);
	return (uint8_t *)rb->array + pos * rb->esize;
}

ssize_t rb_available(const struct rb *rb)
{
	if (!rb)
		return -EINVAL;

	return rbi_available(&rb->rbi);
}

bool rb_is_empty(const struct rb *rb)
{
	if (!rb)
		return true;

	return rbi_is_empty(&rb->rbi);
}

bool rb_is_full(const struct rb *rb)
{
	if (!rb)
		return true;

	return rbi_is_full(&rb->rbi);
}

ssize_t rb_capacity(const struct rb *rb)
{
	if (!rb)
		return -EINVAL;

	return rbi_capacity(&rb->rbi);
}
