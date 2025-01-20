// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#include <gtest/gtest.h>
#include <mm/rbi.h>

class RBITest : public ::testing::Test {
    protected:
	struct rbi *ring_buffer;
	size_t size;

	void SetUp() override
	{
		size = 8; // Must be a power of 2
		ring_buffer = (struct rbi *)malloc(sizeof(struct rbi));
		ASSERT_NE(ring_buffer, nullptr);
		ASSERT_EQ(rbi_init(ring_buffer, size), 0);
	}

	void TearDown() override
	{
		free(ring_buffer);
	}
};

TEST_F(RBITest, Initialization)
{
	EXPECT_EQ(rbi_init(nullptr, size), -EINVAL);
	EXPECT_EQ(rbi_init(ring_buffer, 7), -ENOTSUP); // Not a power of 2
}

TEST_F(RBITest, AvailableElements)
{
	EXPECT_EQ(rbi_available(nullptr), -EINVAL);
	EXPECT_EQ(rbi_available(ring_buffer), 0);
}

TEST_F(RBITest, IsEmpty)
{
	EXPECT_TRUE(rbi_is_empty(ring_buffer));
	EXPECT_TRUE(rbi_is_empty(nullptr));
}

TEST_F(RBITest, IsFull)
{
	EXPECT_FALSE(rbi_is_full(ring_buffer));
	EXPECT_TRUE(rbi_is_full(nullptr));
}

TEST_F(RBITest, PutElement)
{
	EXPECT_EQ(rbi_put(nullptr), -EINVAL);
	for (size_t i = 0; i < size; ++i) {
		EXPECT_EQ(rbi_put(ring_buffer), i);
	}
	EXPECT_EQ(rbi_put(ring_buffer), -ENOSPC); // Buffer should be full
}

TEST_F(RBITest, GetElement)
{
	EXPECT_EQ(rbi_get(nullptr), -EINVAL);
	EXPECT_EQ(rbi_get(ring_buffer), -ENODATA); // Buffer is empty

	rbi_put(ring_buffer);
	EXPECT_EQ(rbi_get(ring_buffer), 0);
	EXPECT_EQ(rbi_get(ring_buffer),
		  -ENODATA); // Buffer should be empty again
}

TEST_F(RBITest, PeekElement)
{
	EXPECT_EQ(rbi_peek(nullptr), -EINVAL);
	EXPECT_EQ(rbi_peek(ring_buffer), -ENODATA); // Buffer is empty

	rbi_put(ring_buffer);
	EXPECT_EQ(rbi_peek(ring_buffer), 0);
	EXPECT_EQ(rbi_peek(ring_buffer),
		  0); // Peek should not remove the element
}

TEST_F(RBITest, Capacity)
{
	EXPECT_EQ(rbi_capacity(nullptr), -EINVAL);
	EXPECT_EQ(rbi_capacity(ring_buffer), size);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}