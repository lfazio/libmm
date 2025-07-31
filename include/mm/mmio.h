#pragma once

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>

#ifdef CONFIG_MOCK_MMIO
#include <mm/mock_mmio.h>

#define DECLARE_MMMIO32() static struct mock_mmio32 mmmio32

#define readb(addr) mock_mmio_read8(&mmmio32,  addr)
#define reads(addr) mock_mmio_read16(&mmmio32, addr)
#define readl(addr) mock_mmio_read32(&mmmio32, addr)

#define writeb(addr, value) mock_mmio_write8(&mmmio32,  addr, value)
#define writes(addr, value) mock_mmio_write16(&mmmio32, addr, value)
#define writel(addr, value) mock_mmio_write32(&mmmio32, addr, value)
#else
#define readb(addr) mmio_read8(NULL,  addr)
#define reads(addr) mmio_read16(NULL, addr)
#define readl(addr) mmio_read32(NULL, addr)

#define writeb(addr, value) mmio_write8(NULL,  addr, value)
#define writes(addr, value) mmio_write16(NULL, addr, value)
#define writel(addr, value) mmio_write32(NULL, addr, value)
#endif

static inline uint8_t mmio_read8(void *unused, uintptr_t addr)
{
    return *(volatile uint8_t *)addr;
}

static inline void mmio_write8(void *unused, uintptr_t addr, uint8_t value)
{
    *(volatile uint8_t *)addr = value;
}

static inline uint16_t mmio_read16(void *unused, uintptr_t addr)
{
    return *(volatile uint16_t *)addr;
}

static inline void mmio_write16(void *unused, uintptr_t addr, uint16_t value)
{
    *(volatile uint16_t *)addr = value;
}

static inline uint32_t mmio_read32(void *unused, uintptr_t addr)
{
    return *(volatile uint32_t *)addr;
}

static inline void mmio_write32(void *unused, uintptr_t addr, uint32_t value)
{
    *(volatile uint32_t *)addr = value;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
