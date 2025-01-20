// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

#include <gtest/gtest.h>
#include <mm/rb.h>

class RBTest : public ::testing::Test {
protected:
    struct rb ring_buffer;
    int *array;
    size_t esize;
    size_t ecount;

    void SetUp() override {
        esize = sizeof(int);
        ecount = 8; // Must be a power of 2
        array = (int *)malloc(esize * ecount);
        ASSERT_NE(array, nullptr);
        ASSERT_EQ(rb_init(&ring_buffer, array, esize, ecount), 0);
    }

    void TearDown() override {
        free(array);
    }
};

TEST_F(RBTest, Initialization) {
    EXPECT_EQ(rb_init(nullptr, array, esize, ecount), -EINVAL);
    EXPECT_EQ(rb_init(&ring_buffer, nullptr, esize, ecount), -EINVAL);
    EXPECT_EQ(rb_init(&ring_buffer, array, esize, 7), -ENOTSUP); // Not a power of 2
    EXPECT_EQ(rb_init(&ring_buffer, array, 0, 7), -EINVAL);
    EXPECT_EQ(rb_init(&ring_buffer, array, esize, 0), -EINVAL);
}

TEST_F(RBTest, PutElement) {
    int element = 42;
    EXPECT_FALSE(rb_put(nullptr, &element));
    for (size_t i = 0; i < ecount; ++i) {
        EXPECT_TRUE(rb_put(&ring_buffer, &element));
    }
    EXPECT_FALSE(rb_put(&ring_buffer, &element)); // Buffer should be full
}

TEST_F(RBTest, GetElement) {
    int element = 42;
    int retrieved_element;
    EXPECT_FALSE(rb_get(nullptr, &retrieved_element));
    EXPECT_FALSE(rb_get(&ring_buffer, &retrieved_element)); // Buffer is empty

    rb_put(&ring_buffer, &element);
    EXPECT_TRUE(rb_get(&ring_buffer, &retrieved_element));
    EXPECT_EQ(retrieved_element, element);
    EXPECT_FALSE(rb_get(&ring_buffer, &retrieved_element)); // Buffer should be empty again
}

TEST_F(RBTest, PeekElement) {
    int element = 42;
    EXPECT_EQ(rb_peek(nullptr), nullptr);
    EXPECT_EQ(rb_peek(&ring_buffer), nullptr); // Buffer is empty

    rb_put(&ring_buffer, &element);
    EXPECT_EQ(*(int *)rb_peek(&ring_buffer), element);
    EXPECT_EQ(*(int *)rb_peek(&ring_buffer), element); // Peek should not remove the element
}

TEST_F(RBTest, AvailableElements) {
    EXPECT_EQ(rb_available(nullptr), -EINVAL);
    EXPECT_EQ(rb_available(&ring_buffer), 0);

    int element = 42;
    rb_put(&ring_buffer, &element);
    EXPECT_EQ(rb_available(&ring_buffer), 1);
}

TEST_F(RBTest, IsEmpty) {
    EXPECT_TRUE(rb_is_empty(&ring_buffer));
    EXPECT_TRUE(rb_is_empty(nullptr));

    int element = 42;
    rb_put(&ring_buffer, &element);
    EXPECT_FALSE(rb_is_empty(&ring_buffer));
}

TEST_F(RBTest, IsFull) {
    EXPECT_FALSE(rb_is_full(&ring_buffer));
    EXPECT_TRUE(rb_is_full(nullptr));

    int element = 42;
    for (size_t i = 0; i < ecount; ++i) {
        rb_put(&ring_buffer, &element);
    }
    EXPECT_TRUE(rb_is_full(&ring_buffer));
}

TEST_F(RBTest, Capacity) {
    EXPECT_EQ(rb_capacity(nullptr), -EINVAL);
    EXPECT_EQ(rb_capacity(&ring_buffer), ecount);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}