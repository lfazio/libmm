// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024-2025 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define _GNU_SOURCE
#include <unistd.h> /* gettid */
#include <pthread.h> /* pthread_* */

#ifndef MUTEX_TYPE
/**
 * @def MUTEX_TYPE
 * @brief Type used to store mutex type internal states
 */
#define MUTEX_TYPE pthread_mutex_t
#endif /* !MUTEX_TYPE */

#ifndef MUTEX_INIT
/**
 * @def MUTEX_INIT(mutex)
 *
 * @brief Primitive used to init a mutex, if not available, should be set to (0u)
 * @param[in] mutex The mutex to initialise
 *
 * @return 0 if successful
 * @return any value is considered an error
 */
#define MUTEX_INIT(mutex) pthread_mutex_init(&mutex, NULL)
#endif /* !MUTEX_INIT */

#ifndef MUTEX_DESTROY
/**
 * @def MUTEX_DESTROY(mutex)
 * @brief Primitive used to destroy a mutex, empty if not available
 *
 * @param[in] mutex The mutex to destroy
 */
#define MUTEX_DESTROY(mutex) (void)pthread_mutex_destroy(&mutex)
#endif /* !MUTEX_DESTROY */

#ifndef MUTEX_LOCK
/**
 * @def MUTEX_LOCK(mutex)
 * @brief Primitive used to lock a mutex, empty if not available
 *
 * @param[in] mutex The mutex to lock
 */
#define MUTEX_LOCK(mutex) pthread_mutex_lock(&mutex)
#endif /* !MUTEX_LOCK */

#ifndef MUTEX_UNLOCK
/**
 * @def MUTEX_UNLOCK(mutex)
 * @brief Primitive used to unlock a mutex, empty if not available
 *
 * @param[in] mutex The mutex to unlock
 */
#define MUTEX_UNLOCK(mutex) pthread_mutex_unlock(&mutex)
#endif /* !MUTEX_UNLOCK */

#ifdef __cplusplus
}
#endif /* __cplusplus */
