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

#include <stddef.h>

/**
 * @brief Mallinfo equivalent structure
 */
struct mm_malloc_info {
	size_t uallocated; /**< Total allocated space (bytes) */
	size_t umaxallocated; /**< Total allocated space (bytes) */
	size_t ucount; /**< Total number of allocations */
};

/* --------------------------------------------------------------------------
 * PUBLIC MACROS
 * -------------------------------------------------------------------------- */

/**
 * @def mm_malloc(size)
 * @brief Allocates @a size bytes.
 *
 * @param[in] size Size in bytes
 *
 * @return A pointer to the allocated memory
 */
#if defined(DEBUG)
#define mm_malloc(size) __mm_malloc((size), __FILE__, __LINE__)
#else
#define mm_malloc(size) __mm_malloc((size), "", 0)
#endif

/**
 * @def mm_free(ptr)
 * @brief Free @a ptr.
 * @param ptr Pointer to free
 */
#if defined(DEBUG)
#define mm_free(ptr) __mm_free((ptr), __FILE__, __LINE__)
#else
#define mm_free(ptr) __mm_free((ptr), "", 0)
#endif

/**
 * @brief Allocates @a x bytes on @a nmemb consecutive blocks.
 * @param nmemb The number of consecutive blocks required
 * @param size The size in bytes
 */
#if defined(DEBUG)
#define mm_calloc(nmemb, size) __mm_calloc((nmemb), (size), __FILE__, __LINE__)
#else
#define mm_calloc(nmemb, size) __mm_calloc((nmemb), (size), "", 0)
#endif

/**
 * @brief Reallocates @a x bytes to @a ptr
 * @param ptr The pointer that needs reallocation
 * @param size The new size in bytes
 */
#if defined(DEBUG)
#define mm_realloc(ptr, size) __mm_realloc((ptr), (size), __FILE__, __LINE__)
#else
#define mm_realloc(ptr, size) __mm_realloc((ptr), (size), "", 0)
#endif

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * @brief Allocate size bytes and store information related to the allocation
 *
 * @param size Requested memory size in bytes
 * @param file The file malloc is called from
 * @param line The line in the file malloc is called from
 * @return void*
 *
 * @note @c file and @c line are only used for debugging when memory tracking is enabled
 * @sa memtrack
 *
 */
void *__mm_malloc(size_t size, const char *file, const int line);

/**
 * @brief Free a pointer and store information related to the free
 *
 * @param ptr Pointer to memory block to free
 * @param file The file free is called from
 * @param line The line in the file free is called from
 *
 * @note @c file and @c line are only used for debugging when memory tracking is enabled
 */
void __mm_free(void *ptr, const char *file, const int line);

/**
 * @brief Like calloc() but keep information about calloc() context
 *
 * @param nmemb The number of consecutive blocks required
 * @param size The size in bytes of each block
 * @param file The file calloc is called from
 * @param line The line in the file calloc is called from
 * @return void*
 *
 * @note @c file and @c line are only used for debugging when memory tracking is enabled
 */
void *__mm_calloc(size_t nmemb, size_t size, const char *file, const int line);

/**
 * @brief Like realloc() but keep information about realloc() context
 *
 * @param ptr Pointer to the memory block that needs to be reallocated
 * @param size The new size in bytes
 * @param file The file realloc is called from
 * @param line The line in the file realloc is called from
 * @return void*
 *
 * @note @c file and @c line are only used for debugging when memory tracking is enabled
 */
void *__mm_realloc(void *ptr, size_t size, const char *file, const int line);

/**
 * @brief Get memory allocation information
 * 
 * @return struct mm_malloc_info 
 */
struct mm_malloc_info mm_malloc_info(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
