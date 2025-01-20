// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

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

#include <mm/rbi.h>

/* --------------------------------------------------------------------------
 * PUBLIC TYPES
 * -------------------------------------------------------------------------- */

/**
 * @brief Ring buffer structure
 */
struct rb {
	struct rbi rbi; /**< Ring buffer index structure */
	void *array; /**< Pointer to the array storing the elements */
	size_t esize; /**< Size of each element in the array */
};

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * @brief Initialize a ring buffer
 * 
 * @param rb Pointer to the ring buffer structure
 * @param array Pointer to the array to be used as the ring buffer
 * @param esize Size of each element in the array
 * @param ecount Total number of elements in the array, must be a power of 2
 * @return 0 on success
 * @return -EINVAL if rb or array is NULL
 * @return -ENOTSUP if ecount is not a power of 2
 */
int rb_init(struct rb *rb, void *array, size_t esize, size_t ecount);

/**
 * @brief Put an element into the ring buffer
 * 
 * @param rb Pointer to the ring buffer structure
 * @param element Pointer to the element to put into the ring buffer
 * @return true on success
 * @return false if the ring buffer is full or if rb is NULL
 */
bool rb_put(struct rb *rb, void *element);

/**
 * @brief Get an element from the ring buffer
 * 
 * @param rb Pointer to the ring buffer structure
 * @param element Pointer to the element to get from the ring buffer
 * @return true on success
 * @return false if the ring buffer is empty or if rb is NULL
 */
bool rb_get(struct rb *rb, void *element);

/**
 * @brief Peek at the next element in the ring buffer without removing it
 * 
 * @param rb Pointer to the ring buffer structure
 * @return Pointer to the element, or NULL if the ring buffer is empty or if rb is NULL
 */
void *rb_peek(struct rb *rb);

/**
 * @brief Get the number of available elements in the ring buffer
 * 
 * @param rb Pointer to the ring buffer structure
 * @return Number of available elements, or -EINVAL if rb is NULL
 */
ssize_t rb_available(struct rb *rb);

/**
 * @brief Check if the ring buffer is empty
 * 
 * @param rb Pointer to the ring buffer structure
 * @return true if the ring buffer is empty or if rb is NULL, false otherwise
 */
bool rb_is_empty(struct rb *rb);

/**
 * @brief Check if the ring buffer is full
 * 
 * @param rb Pointer to the ring buffer structure
 * @return true if the ring buffer is full or if rb is NULL, false otherwise
 */
bool rb_is_full(struct rb *rb);

/**
 * @brief Get the capacity of the ring buffer
 * 
 * @param rb Pointer to the ring buffer structure
 * @return The capacity of the ring buffer, or -EINVAL if rb is NULL
 */
ssize_t rb_capacity(struct rb *rb);

#ifdef __cplusplus
}
#endif /* __cplusplus */
