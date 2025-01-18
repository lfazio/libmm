// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#include <gtest/gtest.h>

#include <mm/slab.h> // Include the header for the functions you want to test

// Test case for mm_slab_create and mm_slab_destroy
TEST(SlabTest, CreateAndDestroyInvalid)
{
	struct mm_slab *slab;

	// Initialize the slab pool before each test
	slab = mm_slab_create(nullptr, 7, 128, 10);
	ASSERT_EQ(slab, nullptr);

	// Initialize the slab pool before each test
	slab = mm_slab_create(nullptr, 0, 0, 10);
	ASSERT_EQ(slab, nullptr);

	// Initialize the slab pool before each test
	slab = mm_slab_create(nullptr, 0, 128, 0);
	ASSERT_EQ(slab, nullptr);
}

TEST(SlabTest, AllocInvalid)
{
	void *ptr = mm_slab_alloc(nullptr);
	ASSERT_EQ(ptr, nullptr);
}

TEST(SlabTest, FreeInvalid)
{
	// Initialize the slab pool before each test
	int err = mm_slab_free(nullptr, nullptr);
	ASSERT_EQ(err, -EINVAL);
}

TEST(SlabTest, DestroyInvalid)
{
	// Initialize the slab pool before each test
	int err = mm_slab_destroy(nullptr);
	ASSERT_EQ(err, -EINVAL);
}

TEST(SlabTest, StatInvalid)
{
	// Initialize the slab pool before each test
	int err = mm_slab_stats(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	ASSERT_EQ(err, -EINVAL);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}