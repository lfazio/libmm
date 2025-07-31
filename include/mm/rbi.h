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

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

/* --------------------------------------------------------------------------
 * PUBLIC TYPES
 * -------------------------------------------------------------------------- */

struct rbi {
	size_t head;
	size_t tail;
	size_t mask;
};

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * @brief Initialize a ring buffer index
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @param size Size of the ring buffer, must be a power of 2
 * @return 0 on success
 * @return -EINVAL if @a rbi is NULL
 * @return -ENOTSUP if @a size is not a power of 2
 */
int rbi_init(struct rbi *rbi, size_t size);

/**
 * @brief Get the number of available elements in the ring buffer
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @return Number of available elements
 * @return  -EINVAL if @a rbi is NULL
 */
ssize_t rbi_available(const struct rbi *rbi);

/**
 * @brief Check if the ring buffer is empty
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @return true if the ring buffer is empty or if @a rbi is NULL, false otherwise
 */
bool rbi_is_empty(const struct rbi *rbi);

/**
 * @brief Check if the ring buffer is full
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @return true if the ring buffer is full or if rbi is NULL, false otherwise
 */
bool rbi_is_full(const struct rbi *rbi);

/**
 * @brief Get an element from the ring buffer
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @return Index of the element
 * @return -EINVAL if @a rbi is NULL
 * @return -ENODATA if the ring buffer is empty
 */
ssize_t rbi_get(struct rbi *rbi);

/**
 * @brief Peek at the next element in the ring buffer without removing it
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @return Index of the element
 * @return -EINVAL if @a rbi is NULL
 * @return -ENODATA if the ring buffer is empty
 */
ssize_t rbi_peek(const struct rbi *rbi);

/**
 * @brief Put an element into the ring buffer
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @return 0 on success
 * @return -EINVAL if @a rbi is NULL
 * @return -ENOSPC if the ring buffer is full
 */
ssize_t rbi_put(struct rbi *rbi);

/**
 * @brief Get the capacity of the ring buffer
 * 
 * @param rbi Pointer to the ring buffer index structure
 * @return The capacity of the ring buffer,
 * @return -EINVAL if @a rbi is NULL
 */
ssize_t rbi_capacity(const struct rbi *rbi);

#ifdef __cplusplus
}
#endif /* __cplusplus */
