// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#include <gtest/gtest.h>

#include <mm/alloc.h>
#include <mm/slab_arena.h>
#include <mm/slab.h>

// Test fixture for slab arena tests
class SlabArenaTest : public ::testing::Test {
protected:
	struct mm_slab_arena_config config[2];
	size_t count = 2;

	void SetUp() override {
		config[0].esize = 128;
		config[0].ecount = 10;
		config[1].esize = 256;
		config[1].ecount = 5;
	}

	void TearDown() override {
	}
};

// Test case for mm_slab_arena_create and mm_slab_arena_destroy
TEST_F(SlabArenaTest, CreateAndDestroy) {
	int result = mm_slab_arena_create(NULL, 1);
	EXPECT_EQ(result, -EINVAL);

	result = mm_slab_arena_create(config, 0);
	EXPECT_EQ(result, -EINVAL);

	result = mm_slab_arena_create(config, count);
	EXPECT_EQ(result, 0);

	result = mm_slab_arena_destroy();
	EXPECT_EQ(result, 0);
}

// Test case for mm_slab_arena_malloc and mm_slab_arena_free with multiple allocations
TEST_F(SlabArenaTest, MultipleAllocAndFree) {
	void *ptrs[15];

	ptrs[0] = mm_slab_arena_malloc(128);
	ASSERT_EQ(ptrs[0], nullptr);

	int result = mm_slab_arena_free((void *)0x1);
	EXPECT_EQ(result, -EINVAL);

	result = mm_slab_arena_free(nullptr);
	EXPECT_EQ(result, -EINVAL);

	result = mm_slab_arena_create(config, count);
	ASSERT_EQ(result, 0);

	ptrs[0] = mm_slab_arena_malloc(0);
	ASSERT_EQ(ptrs[0], nullptr);

	for (int i = 0; i < 15; ++i) {
		ptrs[i] = mm_slab_arena_malloc(128);
		ASSERT_NE(ptrs[i], nullptr);
	}

	for (int i = 0; i < 15; ++i) {
		result = mm_slab_arena_free(ptrs[i]);
		EXPECT_EQ(result, 0);
	}

	result = mm_slab_arena_destroy();
	EXPECT_EQ(result, 0);
}

// Test case for mm_slab_arena_malloc and mm_slab_arena_free with multiple allocations
TEST_F(SlabArenaTest, MultipleCallocAndFree) {
	void *ptrs[15];

	ptrs[0] = mm_slab_arena_calloc(2, 120);
	ASSERT_EQ(ptrs[0], nullptr);

	int result = mm_slab_arena_create(config, count);
	ASSERT_EQ(result, 0);

	ptrs[0] = mm_slab_arena_calloc(0, 120);
	ASSERT_EQ(ptrs[0], nullptr);

	ptrs[0] = mm_slab_arena_calloc(1, 0);
	ASSERT_EQ(ptrs[0], nullptr);

	for (int i = 0; i < 15; ++i) {
		ptrs[i] = mm_slab_arena_calloc(1, 128);
		ASSERT_NE(ptrs[i], nullptr);
	}

	for (int i = 0; i < 15; ++i) {
		result = mm_slab_arena_free(ptrs[i]);
		EXPECT_EQ(result, 0);
	}

	result = mm_slab_arena_destroy();
	EXPECT_EQ(result, 0);
}

// Test case for mm_slab_arena_stats
TEST_F(SlabArenaTest, SlabArenaStats) {
	int result = mm_slab_arena_create(config, count);
	ASSERT_EQ(result, 0);

	struct mm_slab_arena_stats *stats;
	size_t count;

	result = mm_slab_arena_stats(&stats, &count);
	EXPECT_EQ(result, 2);
	EXPECT_EQ(count, 2);

	EXPECT_EQ(stats[0].esize, 128);
	EXPECT_EQ(stats[0].ecount, 10);
	EXPECT_EQ(stats[0].allocated, 0);
	EXPECT_EQ(stats[0].missed, 0);
	EXPECT_EQ(stats[0].freed, 0);

	EXPECT_EQ(stats[1].esize, 256);
	EXPECT_EQ(stats[1].ecount, 5);
	EXPECT_EQ(stats[1].allocated, 0);
	EXPECT_EQ(stats[1].missed, 0);
	EXPECT_EQ(stats[1].freed, 0);

	mm_free(stats);

	result = mm_slab_arena_destroy();
	EXPECT_EQ(result, 0);
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}