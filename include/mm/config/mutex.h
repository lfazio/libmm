// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define _GNU_SOURCE
#include <unistd.h> /* gettid */
#include <pthread.h> /* pthread_* */

#ifndef MUTEX_TYPE
#define MUTEX_TYPE pthread_mutex_t
#endif /* !MUTEX_TYPE */

#ifndef MUTEX_INIT
#define MUTEX_INIT(mutex) pthread_mutex_init(&mutex, NULL)
#endif /* !MUTEX_INIT */

#ifndef MUTEX_DESTROY
#define MUTEX_DESTROY(mutex) pthread_mutex_destroy(&mutex)
#endif /* !MUTEX_DESTROY */

#ifndef MUTEX_LOCK
#define MUTEX_LOCK(mutex) pthread_mutex_lock(&mutex)
#endif /* !MUTEX_LOCK */

#ifndef MUTEX_UNLOCK
#define MUTEX_UNLOCK(mutex) pthread_mutex_unlock(&mutex)
#endif /* !MUTEX_UNLOCK */

#ifdef __cplusplus
}
#endif /* __cplusplus */