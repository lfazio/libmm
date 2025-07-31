#include <errno.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mm/mock_mmio.h>

int mock_mmio_setup(struct mock_mmio32 *mock, const char *name,
                    const char *desc[32], const uint32_t *values_on_reset,
                    uintptr_t start_addr, uintptr_t end_addr)
{
    if (mock == NULL) {
        return -EINVAL;
    }

    mock->name = strdup(name);
    mock->desc = desc;
    mock->reset_values = values_on_reset;
    mock->values = malloc(end_addr - start_addr);
    mock->addr = start_addr;
    mock->length = end_addr - start_addr;
    if (mock->values == NULL) {
        return -ENOMEM;
    }

    return mock_mmio_reset(mock);
}

int mock_mmio_teardown(struct mock_mmio32 *mock)
{
    if (mock == NULL) {
        return -EINVAL;
    }

    free(mock->name);
    free(mock->values);

    return 0;
}

void mock_mmio_display(struct mock_mmio32 *mock)
{
    uint32_t *regval;
    uintptr_t i;

    if (mock == NULL) {
        return;
    }

    regval = (uint32_t *)mock->values;
    printf("```\n%s\n", mock->name);
    printf("%-9s31 .... 0  %-32s  Reset Value\n", "Address", "Register Name");
    for (i = 0; i < mock->length / 4; i++) {
        printf("%" PRIxPTR "  %08" PRIx32 "  %-32s  (%08" PRIx32 ")\n",
               mock->addr + i, regval[i], mock->desc ? (const char *)mock->desc + i*32 : "REG",
                mock->reset_values ? mock->reset_values[i] : 0x0u);
    }
    printf("```\n");
}

int mock_mmio_reset(struct mock_mmio32 *mock)
{
    if (mock == NULL) {
        return -EINVAL;
    }

    if (mock->reset_values != NULL) {
        memcpy(mock->values, mock->reset_values, mock->length);
    } else {
        memset(mock->values, 0x00u, mock->length);
    }

    return 0;
}

uint8_t mock_mmio_read8(struct mock_mmio32 *mock, uintptr_t addr)
{
    return mock->values[addr - mock->addr];
}

void mock_mmio_write8(struct mock_mmio32 *mock, uintptr_t addr, uint8_t value)
{
    mock->values[addr - mock->addr] = value;
}

uint16_t mock_mmio_read16(struct mock_mmio32 *mock, uintptr_t addr)
{
    uint16_t value = 0u;

    value |= (uint32_t)mock->values[addr + 1 - mock->addr];
    value <<= 8u;
    value |= (uint32_t)mock->values[addr + 0 - mock->addr];

    return value;
}

void mock_mmio_write16(struct mock_mmio32 *mock, uintptr_t addr, uint16_t value)
{
    mock->values[addr + 1 - mock->addr] = (uint32_t)((value >> 8u) & 0xFFu);
    mock->values[addr + 0 - mock->addr] = (uint32_t)((value >> 0u) & 0xFFu);
}

uint32_t mock_mmio_read32(struct mock_mmio32 *mock, uintptr_t addr)
{
    uint32_t value = 0u;

    value |= (uint32_t)mock->values[addr + 3 - mock->addr];
    value <<= 8u;
    value |= (uint32_t)mock->values[addr + 2 - mock->addr];
    value <<= 8u;
    value |= (uint32_t)mock->values[addr + 1 - mock->addr];
    value <<= 8u;
    value |= (uint32_t)mock->values[addr + 0 - mock->addr];

    return value;
}

void mock_mmio_write32(struct mock_mmio32 *mock, uintptr_t addr, uint32_t value)
{
    mock->values[addr + 3 - mock->addr] = (uint32_t)((value >> 24u) & 0xFFu);
    mock->values[addr + 2 - mock->addr] = (uint32_t)((value >> 16u) & 0xFFu);
    mock->values[addr + 1 - mock->addr] = (uint32_t)((value >> 8u) & 0xFFu);
    mock->values[addr + 0 - mock->addr] = (uint32_t)((value >> 0u) & 0xFFu);
}
