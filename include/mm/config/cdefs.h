// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024-2025 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>

#ifndef ISPOWEROF2
/**
 * @def ISPOWEROF2(num)
 * @brief Determine id @a num is a pozer of two or not
 *
 * @param[in] num The number to check
 *
 * @return true if @a num is a power of two
 * @return false if @a is not a power of two
 */
#define ISPOWEROF2(num)	(((num) & ((num) - 1)) == 0)
#endif /* !ISPOWEROF2 */

#ifndef ROUNDUP
/**
 * @def ROUNDUP(num, align)
 * @brief Round @a val up to @a align
 *
 * @param[in] val The value to roundup
 * @param[in] align The bound to roundup @a val
 *
 * @return The roundup value
 */
#define ROUNDUP(val, align)	\
	((((val) / (align))*(align)) + (((val) % (align)) ? (align) : 0))
#endif /* !ROUNDUP */

#if __has_attribute(__counted_by__)
# define __counted_by(member)  __attribute__((__counted_by__(member)))
#else
# define __counted_by(member)
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */
