// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define _GNU_SOURCE
#include <unistd.h> /* gettid */
#include <pthread.h> /* pthread_* */

#ifndef THREAD_GETSPECIFIC
#define THREAD_GETSPECIFIC(key) pthread_getspecific(key)
#endif /* !THREAD_GETSPECIFIC */

#ifndef THREAD_SETSPECIFIC
#define THREAD_SETSPECIFIC(key, value) pthread_setspecific(key, value)
#endif /* !THREAD_SETSPECIFIC */

#ifndef THREAD_GETTID
#define THREAD_GETTID() gettid()
#endif /* !THREAD_GETTID */

#ifndef THREAD_KEY_CREATE
#define THREAD_KEY_CREATE(key, destructor) pthread_key_create((pthread_key_t *)key, destructor)
#endif /* !THREAD_KEY_CREATE */

#ifndef THREAD_GET_NAME_MAXLEN
#define THREAD_GET_NAME_MAXLEN 16
#endif /* !THREAD_GET_NAME_MAXLEN */

#ifndef THREAD_GET_NAME
#define THREAD_GET_NAME(tid) "<noname>" /* Not implemented */
#endif /* !THREAD_GET_NAME */

#ifdef __cplusplus
}
#endif /* __cplusplus */
