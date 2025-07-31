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

#include <stdbool.h>

/* --------------------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------------------- */

/**
 * @brief Activates memory tracking.
 *
 * @return 0 on success, non-zero on failure.
 */
int mm_mt_activate(void);

/**
 * @brief Deactivates memory tracking.
 */
void mm_mt_deactivate(void);

/**
 * @brief Provides a summary of memory usage for a specific thread.
 *
 * @param tid Thread ID.
 * @param thread_name Name of the thread.
 * @param verbose If true, provides a verbose summary.
 * @param _puts Function pointer to a custom print function.
 * @param ctx Context for the custom print function.
 * @return 0 on success, non-zero on failure.
 */
int mm_mt_summary_for_thread(int tid, const char *thread_name, bool verbose,
                 int (*_puts)(void *ctx, const char *str),
                 void *ctx);

/**
 * @brief Provides a summary of memory usage.
 *
 * @param verbose If true, provides a verbose summary.
 * @param _puts Function pointer to a custom print function.
 * @param ctx Context for the custom print function.
 * @return 0 on success, non-zero on failure.
 */
int mm_mt_summary(bool verbose, int (*_puts)(void *ctx, const char *str), void *ctx);

#ifdef __cplusplus
}
#endif /* __cplusplus */