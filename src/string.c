// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <string.h>

#include <mm/alloc.h>
#include <mm/string.h>

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

char *mm_strdup(const char *str)
{
	if (!str)
		return NULL;

	return mm_strndup(str, strlen(str));
}

char *mm_strndup(const char *str, size_t n)
{
	char *dup;
	size_t len;

	if (!str)
		return NULL;

	len = strlen(str);
	if (n < len)
		len = n;

	dup = mm_malloc(len + 1);
	if (!dup)
		return NULL;
	dup[len] = '\0';

	mm_memcpy(dup, str, len);

	return dup;
}

void *mm_memcpy(void *dest, const void *src, size_t n)
{
	return memcpy(dest, src, n);
}

void *mm_memmove(void *dest, const void *src, size_t n)
{
	return memmove(dest, src, n);
}
