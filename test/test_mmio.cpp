// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <errno.h>
#include <stddef.h>

#include <gtest/gtest.h>

#define CONFIG_MOCK_MMIO
#include <mm/mmio.h> // Include the header for the functions you want to test

DECLARE_MMMIO32();

// Test case for
TEST(MmioTest, NoDesc)
{
    ASSERT_EQ(mock_mmio_setup(NULL, "TestBank", NULL, NULL, 0, 4), -EINVAL);
    ASSERT_EQ(mock_mmio_teardown(NULL), -EINVAL);

    ASSERT_EQ(mock_mmio_setup(&mmmio32, "TestBank", NULL, NULL, 0x40000000u,
                              0x40000004u),
              0);
    ASSERT_EQ(mock_mmio_teardown(&mmmio32), 0);

    ASSERT_EQ(mock_mmio_setup(&mmmio32, "TestBank", NULL, NULL, 0x40000000u,
                              0x40000004u),
              0);
    ASSERT_EQ(readb(0x40000000u), 0x0u);
    ASSERT_EQ(readb(0x40000001u), 0x0u);
    ASSERT_EQ(readb(0x40000002u), 0x0u);
    ASSERT_EQ(readb(0x40000003u), 0x0u);
    ASSERT_EQ(reads(0x40000000u), 0x0u);
    ASSERT_EQ(reads(0x40000002u), 0x0u);
    ASSERT_EQ(readl(0x40000000u), 0x0u);

    writeb(0x40000000u, 0x01);
    writeb(0x40000001u, 0x02);
    writeb(0x40000002u, 0x03);
    writeb(0x40000003u, 0x04);
    ASSERT_EQ(readl(0x40000000u), 0x04030201u);

    ASSERT_EQ(reads(0x40000000u), 0x0201u);
    ASSERT_EQ(reads(0x40000002u), 0x0403u);

    ASSERT_EQ(readb(0x40000000u), 0x01u);
    ASSERT_EQ(readb(0x40000001u), 0x02u);
    ASSERT_EQ(readb(0x40000002u), 0x03u);
    ASSERT_EQ(readb(0x40000003u), 0x04u);

    mock_mmio_display(NULL);
    mock_mmio_display(&mmmio32);

    ASSERT_EQ(mock_mmio_reset(NULL), -EINVAL);
    ASSERT_EQ(mock_mmio_reset(&mmmio32), 0);

    ASSERT_EQ(readb(0x40000000u), 0x0u);
    ASSERT_EQ(reads(0x40000000u), 0x0u);
    ASSERT_EQ(readl(0x40000000u), 0x0u);

    ASSERT_EQ(mock_mmio_teardown(&mmmio32), 0u);
}

TEST(MmioTest, WithDesc)
{
    uint32_t test_bank_on_reset = 0x12345678;
    const char desc[][32] = {
        "REG_4000'0000",
    };

    ASSERT_EQ(mock_mmio_setup(NULL, "TestBank", (const char **)desc,
                              &test_bank_on_reset, 0, 4),
              -EINVAL);
    ASSERT_EQ(mock_mmio_teardown(NULL), -EINVAL);

    ASSERT_EQ(mock_mmio_setup(&mmmio32, "TestBank", (const char **)desc,
                              &test_bank_on_reset, 0x40000000u, 0x40000004u),
              0);
    ASSERT_EQ(mock_mmio_teardown(&mmmio32), 0);

    ASSERT_EQ(mock_mmio_setup(&mmmio32, "TestBank", (const char **)desc,
                              &test_bank_on_reset, 0x40000000u, 0x40000004u),
              0);
    ASSERT_EQ(readb(0x40000000u), 0x78u);
    ASSERT_EQ(readb(0x40000001u), 0x56u);
    ASSERT_EQ(readb(0x40000002u), 0x34u);
    ASSERT_EQ(readb(0x40000003u), 0x12u);
    ASSERT_EQ(reads(0x40000000u), 0x5678u);
    ASSERT_EQ(reads(0x40000002u), 0x1234u);
    ASSERT_EQ(readl(0x40000000u), 0x12345678u);

    writeb(0x40000000u, 0x01);
    writeb(0x40000001u, 0x02);
    writeb(0x40000002u, 0x03);
    writeb(0x40000003u, 0x04);
    ASSERT_EQ(readl(0x40000000u), 0x04030201u);

    ASSERT_EQ(reads(0x40000000u), 0x0201u);
    ASSERT_EQ(reads(0x40000002u), 0x0403u);

    ASSERT_EQ(readb(0x40000000u), 0x01u);
    ASSERT_EQ(readb(0x40000001u), 0x02u);
    ASSERT_EQ(readb(0x40000002u), 0x03u);
    ASSERT_EQ(readb(0x40000003u), 0x04u);

    mock_mmio_display(&mmmio32);

    ASSERT_EQ(mock_mmio_reset(&mmmio32), 0);

    ASSERT_EQ(readb(0x40000000u), 0x78u);
    ASSERT_EQ(reads(0x40000000u), 0x5678u);
    ASSERT_EQ(readl(0x40000000u), 0x12345678u);

    writes(0x40000000u, 0x0201);
    writes(0x40000002u, 0x0403);
    ASSERT_EQ(readl(0x40000000u), 0x04030201u);

    ASSERT_EQ(reads(0x40000000u), 0x0201u);
    ASSERT_EQ(reads(0x40000002u), 0x0403u);

    ASSERT_EQ(readb(0x40000000u), 0x01u);
    ASSERT_EQ(readb(0x40000001u), 0x02u);
    ASSERT_EQ(readb(0x40000002u), 0x03u);
    ASSERT_EQ(readb(0x40000003u), 0x04u);

    mock_mmio_display(&mmmio32);

    ASSERT_EQ(mock_mmio_reset(&mmmio32), 0);

    ASSERT_EQ(readb(0x40000000u), 0x78u);
    ASSERT_EQ(reads(0x40000000u), 0x5678u);
    ASSERT_EQ(readl(0x40000000u), 0x12345678u);

    ASSERT_EQ(mock_mmio_reset(&mmmio32), 0);

    ASSERT_EQ(readb(0x40000000u), 0x78u);
    ASSERT_EQ(reads(0x40000000u), 0x5678u);
    ASSERT_EQ(readl(0x40000000u), 0x12345678u);

    writel(0x40000000u, 0x04030201);
    ASSERT_EQ(readl(0x40000000u), 0x04030201u);

    ASSERT_EQ(reads(0x40000000u), 0x0201u);
    ASSERT_EQ(reads(0x40000002u), 0x0403u);

    ASSERT_EQ(readb(0x40000000u), 0x01u);
    ASSERT_EQ(readb(0x40000001u), 0x02u);
    ASSERT_EQ(readb(0x40000002u), 0x03u);
    ASSERT_EQ(readb(0x40000003u), 0x04u);

    mock_mmio_display(&mmmio32);

    ASSERT_EQ(mock_mmio_reset(&mmmio32), 0);

    ASSERT_EQ(readb(0x40000000u), 0x78u);
    ASSERT_EQ(reads(0x40000000u), 0x5678u);
    ASSERT_EQ(readl(0x40000000u), 0x12345678u);

    ASSERT_EQ(mock_mmio_teardown(&mmmio32), 0u);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}