// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024-2025 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

 /**
 * @ingroup mm_config
 */

/**
 * @defgroup config_panic
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef ABORT
/**
 * @def ABORT()
 * @brief Abort an execution.
 *
 * @note Can be an infinite loop if abort not available on your platform
 */
#define ABORT() abort();
#endif /* !ABORT */

#ifndef PANIC
/**
 * @def PANIC(format, file, line, ...)
 * @brief Display a panic message before aborting processing
 *
 * @param[in] format The format of the string to display
 * @param[in] file The filename where it panicked
 * @param[in] line The line where it panicked
 * @param[in] ... The arguments of the @a format to display
 */
#define PANIC(format, file, line, ...) do { \
	printf("PANIC: %s:%d: " format, file, line, __VA_ARGS__); \
	abort(); \
} while (0)
#endif /* !PANIC */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
 * @}
 */
 