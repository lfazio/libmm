// SPDX Licence-Identifier: Apache-2.0
// SPDX-FileCopyrightText: 2024 Laurent Fazio <laurent.fazio@gmail.com>

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */

#include <gtest/gtest.h>

#include <mm/string.h>

TEST(test_mm_strdup_einval, StrdupEinval)
{
	ASSERT_EQ(mm_strdup(NULL), nullptr);
	ASSERT_EQ(mm_strndup(NULL, 5), nullptr);
}

TEST(test_mm_strdup_empty, StrdupEmpty)
{
	char *str;

	str = mm_strdup("");
	ASSERT_NE(str, nullptr);
	ASSERT_EQ(str[0], 0);
	ASSERT_STREQ(str, "");
	mm_free(str);

	str = mm_strndup("Hello, world!", 0);
	ASSERT_NE(str, nullptr);
	ASSERT_EQ(str[0], 0);
	ASSERT_STREQ(str, "");
	mm_free(str);
}

TEST(test_mm_strdup_ok, StrdupOk)
{
	char *str;

	str = mm_strdup("Hello, world!");
	ASSERT_NE(str, nullptr);
	ASSERT_EQ(str[13], 0);
	ASSERT_STREQ(str, "Hello, world!");
	mm_free(str);

	str = mm_strndup("Hello, world!", 13);
	ASSERT_NE(str, nullptr);
	ASSERT_EQ(str[13], 0);
	ASSERT_STREQ(str, "Hello, world!");
	mm_free(str);

	str = mm_strndup("Hello, world!", 5);
	ASSERT_NE(str, nullptr);
	ASSERT_EQ(str[5], 0);
	ASSERT_STREQ(str, "Hello");
	mm_free(str);
}

TEST(test_mm_memcpy, Memcpy)
{
	char src[] = "Hello, world!";
	char dest[14];

	ASSERT_EQ(mm_memcpy(dest, src, 14), dest);
	ASSERT_STREQ(dest, "Hello, world!");
}

TEST(test_mm_memmove, Memmove)
{
	char src[] = "Hello, world!";
	char dest[14];

	ASSERT_EQ(mm_memmove(dest, src, 14), dest);
	ASSERT_STREQ(dest, "Hello, world!");
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
