// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#include <mm/config/cdefs.h>

#include <mm/rbi.h>

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

int rbi_init(struct rbi *rbi, size_t size)
{
	if (!rbi)
		return -EINVAL;

	if (!size || !ISPOWEROF2(size))
		return -ENOTSUP;

	rbi->head = 0;
	rbi->tail = 0;
	rbi->mask = size - 1;

	return 0;
}

ssize_t rbi_available(const struct rbi *rbi)
{
	if (!rbi)
		return -EINVAL;

	return rbi->tail - rbi->head;
}

bool rbi_is_empty(const struct rbi *rbi)
{
	if (!rbi)
		return true;

	return rbi->tail - rbi->head == 0;
}

bool rbi_is_full(const struct rbi *rbi)
{
	if (!rbi)
		return true;

	return !rbi->mask || rbi_available(rbi) > rbi->mask;
}

ssize_t rbi_get(struct rbi *rbi)
{
	size_t head;

	if (!rbi)
		return -EINVAL;

	if (rbi_is_empty(rbi))
		return -ENODATA;

	head = rbi->head;
	rbi->head = head + 1;

	return head & rbi->mask;
}

ssize_t rbi_peek(const struct rbi *rbi)
{
	if (!rbi)
		return -EINVAL;

	if (rbi_is_empty(rbi))
		return -ENODATA;

	return rbi->head & rbi->mask;
}

ssize_t rbi_put(struct rbi *rbi)
{
	size_t tail;

	if (!rbi)
		return -EINVAL;

	if (rbi_is_full(rbi))
		return -ENOSPC;

	tail = rbi->tail;
	rbi->tail = tail + 1;

	return tail & rbi->mask;
}

ssize_t rbi_capacity(const struct rbi *rbi)
{
	if (!rbi)
		return -EINVAL;

	return rbi->mask + 1;
}
