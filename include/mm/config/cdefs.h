// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef ISPOWEROF2
#define ISPOWEROF2(num)	(((num) & ((num) - 1)) == 0)
#endif /* !ISPOWEROF2 */

#ifndef ROUNDUP
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