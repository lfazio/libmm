// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef PANIC
#define PANIC(format, file, line, ...) do { \
	printf("PANIC: %s:%d: " format, (file), (line), __VA_ARGS__); \
	abort(); \
} while (0)
#endif /* !PANIC */

#ifdef __cplusplus
}
#endif /* __cplusplus */