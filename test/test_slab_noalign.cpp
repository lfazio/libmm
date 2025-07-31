// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#include <gtest/gtest.h>

#include <mm/slab.h> // Include the header for the functions you want to test

// Test fixture for slab memory management tests
class SlabTest : public ::testing::Test {
    protected:
	struct mm_slab *slab;

	void SetUp() override
	{
		// Initialize the slab pool before each test
		slab = mm_slab_create(nullptr, 0, 128, 10);
		ASSERT_NE(slab, nullptr);
	}

	void TearDown() override
	{
		// Destroy the slab pool after each test
		int result = mm_slab_destroy(slab);
		ASSERT_EQ(result, 0);
	}
};

// Test case for mm_slab_create and mm_slab_destroy
TEST_F(SlabTest, CreateAndDestroy)
{
	// Slab pool is created in SetUp and destroyed in TearDown
	// Just verify that the setup and teardown were successful
	EXPECT_NE(slab, nullptr);
}

// Test case for mm_slab_alloc and mm_slab_free with multiple allocations
TEST_F(SlabTest, MultipleAllocAndFree)
{
	void *ptrs[10];
	for (int i = 0; i < 10; ++i) {
		ptrs[i] = mm_slab_alloc(slab);
		ASSERT_NE(ptrs[i], nullptr);
	}

	// All slabs should be allocated now
	void *extra_ptr = mm_slab_alloc(slab);
	EXPECT_EQ(extra_ptr, nullptr);

	for (int i = 0; i < 10; ++i) {
		int result = mm_slab_free(slab, ptrs[i]);
		EXPECT_EQ(result, 0);
	}

	// All slabs should be freed now
	for (int i = 0; i < 10; ++i) {
		ptrs[i] = mm_slab_alloc(slab);
		ASSERT_NE(ptrs[i], nullptr);
	}

	for (int i = 0; i < 10; ++i) {
		int result = mm_slab_free(slab, ptrs[i]);
		EXPECT_EQ(result, 0);
	}
}

// Test case for mm_slab_stats
TEST_F(SlabTest, SlabStats)
{
	size_t esize, ecount, allocated, missed, freed;
	uint8_t buffer[1280];

	mm_slab_destroy(slab);
	slab = mm_slab_create(buffer, 0, 128, 10);

	int result = mm_slab_stats(slab, &esize, &ecount, &allocated, &missed,
				   &freed);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(esize, 128);
	EXPECT_EQ(ecount, 10);
	EXPECT_EQ(allocated, 0);
	EXPECT_EQ(missed, 0);
	EXPECT_EQ(freed, 0);

	void *ptr = mm_slab_alloc(slab);
	ASSERT_NE(ptr, nullptr);

	result = mm_slab_stats(slab, &esize, &ecount, &allocated, &missed,
			       &freed);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(allocated, 1);

	result = mm_slab_stats(slab, &esize, &ecount, &allocated, &missed,
			       nullptr);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(allocated, 1);

	EXPECT_EQ(mm_slab_destroy(slab), -EAGAIN);

	result = mm_slab_free(slab, nullptr);
	EXPECT_EQ(result, -EINVAL);

	result = mm_slab_free(slab, ptr);
	EXPECT_EQ(result, 0);

	result = mm_slab_stats(slab, &esize, &ecount, &allocated, &missed,
			       &freed);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(freed, 1);

	result = mm_slab_stats(slab, nullptr, &ecount, &allocated, &missed,
			       &freed);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(freed, 1);

	result = mm_slab_stats(slab, &esize, nullptr, &allocated, &missed,
			       &freed);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(freed, 1);

	result = mm_slab_stats(slab, &esize, &ecount, nullptr, &missed,
			       &freed);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(freed, 1);

	result = mm_slab_stats(slab, &esize, &ecount, &allocated, nullptr,
			       &freed);
	EXPECT_EQ(result, 0);
	EXPECT_EQ(freed, 1);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
