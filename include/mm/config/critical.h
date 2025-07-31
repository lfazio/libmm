// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef IRQ_SAVE
#define IRQ_SAVE() 0
#endif /* !IRQ_SAVE */

#ifndef IRQ_RESTORE
#define IRQ_RESTORE(flags) (void)flags
#endif /* !IRQ_RESTORE */

#ifdef __cplusplus
}
#endif /* __cplusplus */
