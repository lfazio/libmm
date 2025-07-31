#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stddef.h>
#include <stdint.h>

struct mock_mmio32 {
    char *name; /* Name of the emulated register bank */
    const char **desc; /* Name of mapped registers */
    uintptr_t addr; /* Start of mapping address */
    size_t length;
    uint8_t *values;
    const uint32_t *reset_values;
};

int mock_mmio_setup(struct mock_mmio32 *mock, const char *name,
                    const char *desc[32], const uint32_t *value_on_reset,
                    uintptr_t start_addr, uintptr_t end_addr);

int mock_mmio_teardown(struct mock_mmio32 *mock);

void mock_mmio_display(struct mock_mmio32 *mock);

int mock_mmio_reset(struct mock_mmio32 *mock);

uint8_t mock_mmio_read8(struct mock_mmio32 *mock, uintptr_t addr);

void mock_mmio_write8(struct mock_mmio32 *mock, uintptr_t addr, uint8_t value);

uint16_t mock_mmio_read16(struct mock_mmio32 *mock, uintptr_t addr);

void mock_mmio_write16(struct mock_mmio32 *mock, uintptr_t addr,
                       uint16_t value);

uint32_t mock_mmio_read32(struct mock_mmio32 *mock, uintptr_t addr);

void mock_mmio_write32(struct mock_mmio32 *mock, uintptr_t addr,
                       uint32_t value);

#ifdef __cplusplus
}
#endif /* __cplusplus */
