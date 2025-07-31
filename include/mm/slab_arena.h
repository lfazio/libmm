// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

/**
 * @ingroup mm_components
 */

/**
 * A slab allocator is useful to manage memory fragmentation. It can stick the
 * fragmentationto the slab pools, which let large chunk of contiguous range
 * available.
 *
 * When an allocation won't fit the current slab element size, it won't allocate
 * a slab from the next pool, it fallbacks to a standard sys_mm_malloc() or
 * sys_mm_calloc().
 *
 * The slab arena statistics can be used to adjust the various pool size for the user
 * application. An application can monitor the exact level of available slabs.
 *
 * In order to create and fine tune a configuration table, start with an initial
 * slab map. Then a developer might monitor the `missed` statistics in order to
 * determine the allocation that has to fall back to default memory allocation
 * function. A developer might also monitor `allocated - freed` to evaluate the
 * occupation of the pools. With those values, you might tune up/down the various
 * pools and let almost large allocation to the default memory allocation.
 *
 * It is mandatory to initialise a slaba rena as in the following example:
 *
 * @code
 *
 * // Sorted by increasing element size
 * struct mm_slab_arena_config _sarena_config[6] = {
 *     { 32, 1024 }, // 1024 buffers for allocation smaller than 32 bytes
 *     { 64, 512 }, // 512 buffers for allocation smaller than 64 bytes
 *     { 128, 256 }, // 256 buffers for allocation smaller than 128 bytes
 *     { 256, 128 }, // 128 buffers for allocation smaller than 256 bytes
 *     { 512, 64 }, // 512 buffers for allocation smaller than 512 bytes
 *     { 1024, 32 }, // 32 buffers for allocation smaller than 1024
 *     { 4096, 32 } // 32 buffers for allocation smaller than 4096
 * };
 *
 * int main(void)
 * {
 *     int err;
 *     uint8_t *ptr;
 *     // ... skipped
 *
 *     err = mm_slab_arena_create(_sarena_config, 6);
 *     // ... skipped
 *     ptr = mm_slab_arena_malloc(47); // alloc 1 buffer from 64-byte-pool
 *
 *     // ... do whatever is needed with ptr then release it
 *     mm_arena_free(ptr);
 *
 *
 *     // ... You can release a kmem if and only if
 *     // ... skipped
 *  }
 * @endcode
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <stdlib.h>

/* --------------------------------------------------------------------------
 * PUBLIC TYPES
 * -------------------------------------------------------------------------- */

/**
 * @brief Configuration of kmem pools
 */
struct mm_slab_arena_config {
	size_t esize; /*<! Element size */
	size_t ecount; /*<! Element count */
};

/**
 * @brief Kmem Pool Statistics
 */
struct mm_slab_arena_stats {
	size_t esize; /*<! Element size */
	size_t ecount; /*<! Element count */
	size_t allocated; /*<! Total number of memory allocations */
	size_t missed; /*<! Total number of missed allocation (allocation that
			    should have happen in this pool but no room left) */
	size_t freed; /*<! Total number of memory release */
};

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * @brief Create a set of memory pools to allocate buffers
 *
 * @param[in] config The configuration of underlying pools (sorted by esize)
 * @param[in] ecount The number of pools in the kmem
 *
 * @return 0 if successful, < 0 otherwise
 */
int mm_slab_arena_create(struct mm_slab_arena_config *config, size_t count);

/**
 * @brief Destroy kmem pools
 *
 * @note Kmem will destroy all empty pools, and return -EAGAIN if any of the pool is
 * not empty.
 *
 * @return 0 if successful, < 0 otherwise
 */
int mm_slab_arena_destroy(void);

/**
 * @brief The mm_slab_arena_malloc() function allocates size bytes and returns a pointer
 *        to the allocated memory. The memory is not initialized.
 *
 * @param[in] size The requested size to allocate
 *
 * @return a pointer to the allocated buffer, NULL if none left in the slab pool
 */
void *mm_slab_arena_malloc(size_t size);

/**
 * @brief The mm_slab_arena_calloc() function allocates memory for an array of nmemb
 *        elements of size bytes each and returns a pointer to the allocated memory.
 *        The memory is set to zero.
 *
 * @param[in] nmemb The number of element
 * @param[in] size The size of one element
 *
 * @return a pointer to the allocated buffer, NULL if none left in the slab pool
 */
void *mm_slab_arena_calloc(size_t nmemb, size_t size);

/**
 * @brief The free() function frees the memory space pointed to by ptr, which must
 *        have been returned by a previous call to malloc() or related functions.
 *        Otherwise, or if ptr has already been freed, undefined behavior occurs.
 *        If ptr is NULL, no operation is performed.
 *
 * @param[in] ptr The pointer to the allocated memory
 *
 * @return 0 if successful, < 0 otherwise
 */
int mm_slab_arena_free(void *ptr);

/**
 * @brief Retrieve stats on the kmem pool
 *
 * @param[out] stats A placeholder for the array to be retrieved
 * @param[out] count the number of pools inside the kmem pool
 *
 * @return 0 if successful, < 0 otherwise
 */
int mm_slab_arena_stats(struct mm_slab_arena_stats **stats, size_t *count);

#ifdef __cplusplus
}
#endif /* __cplusplus */
