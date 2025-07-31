// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <gtest/gtest.h>

#include <mm/alloc.h> // Include the header for the functions you want to test
#include <mm/track.h>

// Test fixture for memory allocation tests
class AllocTest : public ::testing::Test {
    protected:
	void SetUp() override
	{
		// Initialize the context or any setup required before each test
		mm_mt_activate();
	}

	void TearDown() override
	{
		// Clean up after each test
		mm_mt_deactivate();
	}
};

// Test case for mm_malloc
TEST_F(AllocTest, MallocTrack)
{
	struct mm_malloc_info info;

	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	mm_free(ptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 0);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 0);
}

// Test case for double mm_free
TEST_F(AllocTest, DoubleFree)
{
	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	mm_free(ptr);
	EXPECT_EXIT(mm_free(ptr), ::testing::KilledBySignal(SIGABRT), ".*");
}

// Test case for mm_calloc
TEST_F(AllocTest, CallocTrack)
{
	struct mm_malloc_info info;

	void *ptr = mm_calloc(10, 10);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	mm_free(ptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 0);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 0);
}

// Test case for mm_realloc with initial allocation
TEST_F(AllocTest, ReallocInitialAllocationTrack)
{
	struct mm_malloc_info info;

	void *ptr = mm_realloc(nullptr, 100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	mm_free(ptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 0);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 0);
}

// Test case for mm_realloc with reallocation
TEST_F(AllocTest, ReallocReallocationTrack)
{
	struct mm_malloc_info info;

	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	void *new_ptr = mm_realloc(ptr, 200);
	ASSERT_NE(new_ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 200);
	EXPECT_EQ(info.umaxallocated, 200);
	EXPECT_EQ(info.ucount, 1);

	mm_free(new_ptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 0);
	EXPECT_EQ(info.umaxallocated, 200);
	EXPECT_EQ(info.ucount, 0);
}

// Test case for mm_realloc with freeing memory
TEST_F(AllocTest, ReallocFreeMemoryTrack)
{
	struct mm_malloc_info info;

	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	mm_realloc(ptr, 0); // Free the allocated memory
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 0);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 0);
}

int _puts(void *ctx, const char *str) {
	if (ctx == NULL)
		return puts(str);
	std::ostringstream *oss = static_cast<std::ostringstream*>(ctx);
	(*oss) << str;
	return strlen(str);
}

// Test case for mm_mt_summary
TEST_F(AllocTest, MemTrackSummary)
{
	struct mm_malloc_info info;

	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	std::ostringstream oss;
	mm_mt_summary(false, _puts, &oss);
	std::string output = oss.str();
	EXPECT_EQ(output.rfind("{\n\t'current-heap-usage': 100,\n"), 0);
	puts(output.c_str());

	mm_free(ptr);
	std::ostringstream oss2;
	mm_mt_summary(false, _puts, &oss2);
	std::string output2 = oss2.str();
	EXPECT_EQ(output2.rfind("{\n\t'current-heap-usage': 0,\n"), 0);
	puts(output2.c_str());
}

// Test case for mm_mt_summary
TEST_F(AllocTest, MemTrackSummaryVerbose)
{
	struct mm_malloc_info info;

	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	std::ostringstream oss;
	mm_mt_summary(true, _puts, &oss);
	std::string output = oss.str();
	EXPECT_EQ(output.rfind("{\n\t'current-heap-usage': 100,\n"), 0);
	puts(output.c_str());

	mm_free(ptr);
	std::ostringstream oss2;
	mm_mt_summary(true, _puts, &oss2);
	std::string output2 = oss2.str();
	EXPECT_EQ(output2.rfind("{\n\t'current-heap-usage': 0,\n"), 0);
	puts(output2.c_str());
}

// Test case for mm_mt_summary
TEST_F(AllocTest, MemTrackDisabledSummary)
{
	struct mm_malloc_info info;

	mm_mt_deactivate();
	EXPECT_EQ(mm_mt_summary(true, _puts, NULL), -ENOSYS);
	mm_mt_activate();

	EXPECT_EQ(mm_mt_summary(false, NULL, NULL), -EINVAL);
}

// Test case for mm_mt_summary
TEST_F(AllocTest, MemTrackSummaryForThread)
{
	struct mm_malloc_info info;

	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	std::ostringstream oss;
	mm_mt_summary_for_thread(gettid(), "test", false, _puts, &oss);
	std::string output = oss.str();
	EXPECT_EQ(output.rfind("{\n\t'thread': [ 'test',"), 0);
	puts(output.c_str());

	mm_free(ptr);
	std::ostringstream oss2;
	mm_mt_summary_for_thread(gettid(), "test", false, _puts, &oss2);
	std::string output2 = oss2.str();
	EXPECT_EQ(output2.rfind("{\n\t'thread': [ 'test',"), 0);
	puts(output2.c_str());

	mm_mt_summary_for_thread(gettid(), NULL, false, _puts, NULL);
	std::ostringstream oss3;
	mm_mt_summary_for_thread(gettid(), NULL, false, _puts, &oss2);
	std::string output3 = oss3.str();
	EXPECT_NE(output3.rfind("{\t\t'thread': [ '<noname>',"), 0);
	puts(output3.c_str());

	mm_mt_deactivate();
	EXPECT_EQ(mm_mt_summary_for_thread(gettid(), NULL, false, _puts, NULL), -ENOSYS);
	mm_mt_activate();

	EXPECT_EQ(mm_mt_summary_for_thread(-2, NULL, false, _puts, NULL), -EINVAL);
	EXPECT_EQ(mm_mt_summary(false, NULL, NULL), -EINVAL);
}

// Test case for mm_mt_summary
TEST_F(AllocTest, MemTrackSummaryForThreadVerbose)
{
	struct mm_malloc_info info;

	void *ptr = mm_malloc(100);
	ASSERT_NE(ptr, nullptr);
	info = mm_malloc_info();
	EXPECT_EQ(info.uallocated, 100);
	EXPECT_EQ(info.umaxallocated, 100);
	EXPECT_EQ(info.ucount, 1);

	std::ostringstream oss;
	mm_mt_summary_for_thread(gettid(), "test", true, _puts, &oss);
	std::string output = oss.str();
	EXPECT_EQ(output.rfind("{\n\t'thread': [ 'test',"), 0);
	puts(output.c_str());

	mm_free(ptr);
	std::ostringstream oss2;
	mm_mt_summary_for_thread(gettid(), "test", true, _puts, &oss2);
	std::string output2 = oss2.str();
	EXPECT_EQ(output2.rfind("{\n\t'thread': [ 'test',"), 0);
	puts(output2.c_str());
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}