// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

/**
 * @ingroup mm_components
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <mm/alloc.h>

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

/** 
 * @brief Duplicate a string
 *
 * @param[in] str The string to duplicate
 *
 * @return char*
 */
char *mm_strdup(const char *str);

/**
 * @brief Duplicate a string up to @a n characters
 * 
 * @param[out] str String to duplicate
 * @param[in] n Number of characters to duplicate
 *
 * @return The duplicated @a str with maximum size of @a n
 */
char *mm_strndup(const char *str, size_t n);

/**
 * @brief The memcpy() function copies @a n bytes from memory area @a src to memory area @a dest.
 * The memory areas must not overlap.  Use mm_memmove() if the memory areas do overlap.
 * 
 * @param[out] dest Destination
 * @param[in] src Source
 * @param[in] n Number of bytes to copy from @a src to @a dest
 *
 * @return a pointer to @a dest
 */
void *mm_memcpy(void *dest, const void *src, size_t n);

/**
 * @brief The memmove() function copies @a n bytes from memory area @a src to memory area @a dest.
 * The memory areas may overlap: copying takes place as though the bytes in src are first copied 
 * into a temporary array that does not overlap @a src or @a dest, and the bytes are then copied 
 * from the temporary array to @a dest.
 * 
 * @param[out] dest Destination
 * @param[in] src Source
 * @param[in] n Size of the @a dest buffer
 *
 * @return a pointer to @a dest
 */
void *mm_memmove(void *dest, const void *src, size_t n);

#ifdef __cplusplus
}
#endif /* __cplusplus */
