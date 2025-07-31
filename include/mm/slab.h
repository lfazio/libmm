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

#include <stdlib.h>

/* --------------------------------------------------------------------------
 * PUBLIC TYPES
 * -------------------------------------------------------------------------- */

/**
 * @brief Opaque type containing slab allocation
 */
struct mm_slab;

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * @brief Create a memory pool to allocate buffer of @a esize
 *
 * @param[in] buffer The buffer from which the slab will be allocated, if buffer is NULL,
 *            the buffer will be allocated.
 * @param[in] alignment if > 0 and alignement is a power of two, every buffer
 *            allocated will be aligned on an address multiple of alignement as
 *            well as the size also
 * @param[in] esize The size required for a slab element
 * @param[in] ecount The number of slabs in the buffer pool
 *
 * @return an opaque descriptor for the slab pool, NULL otherwise
 */
struct mm_slab *mm_slab_create(void *buffer, size_t alignment, size_t esize, size_t ecount);

/**
 * @brief Destroy a slab pool
 *
 * @param[in] slab The slab pool to destroy
 *
 * @return 0 if successful, a negative value otherwise
 */
int mm_slab_destroy(struct mm_slab *slab);

/**
 * @brief Get one free slab from the slab pool
 *
 * @param[in] slab The buffer pool to allocate from
 *
 * @return a pointer to the allocated buffer, NULL is none left in the slab pool
 */
void *mm_slab_alloc(struct mm_slab *slab);

/**
 * @brief Put back a pointer to the slab pool
 *
 * @param[in] slab The buffer pool to use
 * @param[in] ptr The pointer to the slab to put back into @a slab
 *
 * @return 0 if successful, a negative value otherwise
 */
int mm_slab_free(struct mm_slab *slab, void *ptr);

/**
 * @brief Get stats from a memory pool
 *
 * @param[in] slab The buffer pool to use
 * @param[out] esize The maximum size of one element in the pool
 * @param[out] ecount The number of element in the pool
 * @param[out] allocated The number of allocation in this pool
 * @param[out] missed The number of impossible allocation in this pool
 * @param[out] freed The number of free in this pool
 *
 * @return 0 if successful, a negative value otherwise
 */
int mm_slab_stats(struct mm_slab *slab,
		   size_t *esize, size_t *ecount, size_t *allocated, size_t *missed, size_t *freed);

#ifdef __cplusplus
}
#endif /* __cplusplus */
