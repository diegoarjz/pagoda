#include <common/const_str.h>

#include <gtest/gtest.h>
#include <cstring>

#include <iostream>

using namespace selector;

TEST(ConstString, test_creation)
{
	constexpr const_str<5> s1("Hello");
	static_assert(s1.size() == 5, "Wrong size");

	static_assert(s1[0] == 'H', "Wrong letter");
	static_assert(s1[1] == 'e', "Wrong letter");
	static_assert(s1[2] == 'l', "Wrong letter");
	static_assert(s1[3] == 'l', "Wrong letter");
	static_assert(s1[4] == 'o', "Wrong letter");

	const char* string_value = s1;
	EXPECT_EQ(std::strcmp(string_value, "Hello"), 0);
}

TEST(ConstString, test_concatenation)
{
	constexpr const_str<5> s1("Hello");
	constexpr const_str<6> s2(" World");
	auto result = s1 + s2;

	// static_assert(result.size() == 11, "Wrong size");

	const char* string_value = result;
	EXPECT_EQ(std::strcmp(string_value, "Hello World"), 0);
}

TEST(ConstString, test_concatenation_with_literals)
{
	constexpr const_str<5> s1("Hello");
	constexpr const char literal[] = " World";
	auto result = s1 + literal;

	// static_assert(result.size() == 11, "Wrong size");

	const char* string_value = result;
	EXPECT_EQ(std::strcmp(string_value, "Hello World"), 0);
}

TEST(ConstString, test_concatenation_with_literals_both_ways)
{
	constexpr const char literal[] = "Hello";
	constexpr const_str<6> s1(" World");
	auto result = literal + s1;

	// static_assert(result.size() == 11, "Wrong size");

	const char* string_value = result;
	EXPECT_EQ(std::strcmp(string_value, "Hello World"), 0);
}

TEST(ConstString, test_multiple_concatenations)
{
	constexpr auto s1 = literal("Hello");
	constexpr auto s2 = literal(", ");
	constexpr auto s3 = literal("World");

	const char* string_value = s1 + s2 + s3;

	EXPECT_EQ(std::strcmp(string_value, "Hello, World"), 0);
}
