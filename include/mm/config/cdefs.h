// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef ROUNDUP
#define ROUNDUP(val, align)	\
	((((val) / (align))*(align)) + (((val) % (align)) ? (align) : 0))
#endif /* !ROUNDUP */

#ifdef __cplusplus
}
#endif /* __cplusplus */