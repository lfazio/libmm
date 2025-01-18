// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <gtest/gtest.h>

#include <mm/alloc.h> // Include the header for the functions you want to test
#include <mm/track.h>

// Test case for mm_malloc
TEST(AllocTest, Malloc)
{
	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	mm_free(ptr);
}

// Test case for mm_calloc
TEST(AllocTest, Calloc)
{
	void *ptr = mm_calloc(10, 10);
	ASSERT_NE(ptr, nullptr);
	mm_free(ptr);
}

// Test case for mm_realloc with initial allocation
TEST(AllocTest, ReallocInitialAllocation)
{
	void *ptr = mm_realloc(nullptr, 100);
	ASSERT_NE(ptr, nullptr);
	mm_free(ptr);
}

// Test case for mm_realloc with reallocation
TEST(AllocTest, ReallocReallocation)
{
	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);

	void *new_ptr = mm_realloc(ptr, 200);
	ASSERT_NE(new_ptr, nullptr);

	mm_free(new_ptr);
}

// Test case for mm_realloc with freeing memory
TEST(AllocTest, ReallocFreeMemory)
{
	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);

	mm_realloc(ptr, 0); // Free the allocated memory
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}