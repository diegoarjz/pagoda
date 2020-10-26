#include <pagoda/common/range.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace pagoda;
using namespace pagoda::common;

template<class Type>
class RangeTest : public ::testing::Test
{
};

typedef ::testing::Types<uint32_t, int32_t, uint16_t, int16_t, uint8_t, int8_t> RangeTypes;
TYPED_TEST_SUITE(RangeTest, RangeTypes);

TYPED_TEST(RangeTest, test_start_is_not_equal_to_end)
{
	Range<TypeParam> range(TypeParam(1));

	EXPECT_NE(range.begin(), range.end());
}

TYPED_TEST(RangeTest, test_with_end_value)
{
	Range<TypeParam> range(TypeParam(1));

	auto end = range.end();
	TypeParam i(0);
	for (auto iter = range.begin(); iter != end; ++iter) {
		EXPECT_EQ(*iter, i++);
	}
	EXPECT_EQ(i, 1);
}

TYPED_TEST(RangeTest, test_with_start_and_end_value)
{
	Range<TypeParam> range(TypeParam(0), TypeParam(10));

	auto end = range.end();
	TypeParam i(0);
	for (auto iter = range.begin(); iter != end; ++iter) {
		EXPECT_EQ(*iter, i++);
	}
	EXPECT_EQ(i, 10);
}

TYPED_TEST(RangeTest, test_with_start_end_and_increment_value)
{
	Range<TypeParam> range(TypeParam(0), TypeParam(10), TypeParam(2));

	auto end = range.end();
	TypeParam i(0);
	for (auto iter = range.begin(); iter != end; ++iter) {
		EXPECT_EQ(*iter, i);
		i += 2;
	}
	EXPECT_EQ(i, 10);
}

TYPED_TEST(RangeTest, test_iterator_pre_post_fix_increments)
{
	Range<TypeParam> range(TypeParam(0), TypeParam(10), TypeParam(2));

	auto iter = range.begin();
	EXPECT_EQ(*iter, 0);
	iter++;
	EXPECT_EQ(*iter, 2);
	++iter;
	EXPECT_EQ(*iter, 4);
}

TYPED_TEST(RangeTest, test_range_iteration)
{
	Range<TypeParam> range(TypeParam(10));

	TypeParam i(0);
	for (auto v : range) {
		EXPECT_EQ(v, i++);
	}
	EXPECT_EQ(i, 10u);
}

TYPED_TEST(RangeTest, test_range_iteration_reference)
{
	Range<TypeParam> range(TypeParam(10));

	TypeParam i(0);
	for (auto &v : range) {
		EXPECT_EQ(v, i++);
	}
	EXPECT_EQ(i, 10u);
}

TYPED_TEST(RangeTest, test_range_iteration_const_reference)
{
	Range<TypeParam> range(TypeParam(10));

	TypeParam i(0);
	for (const auto &v : range) {
		EXPECT_EQ(v, i++);
	}
	EXPECT_EQ(i, 10u);
}

TYPED_TEST(RangeTest, test_negative)
{
	Range<TypeParam> range(TypeParam(-10), TypeParam(0));

	TypeParam i(-10);
	for (auto v : range) {
		EXPECT_EQ(v, i++);
	}
	EXPECT_EQ(i, 0u);
}

TYPED_TEST(RangeTest, test_negative_increments)
{
	Range<TypeParam> range(TypeParam(0), TypeParam(-10), TypeParam(-1));

	TypeParam i(0);
	for (auto v : range) {
		EXPECT_EQ(v, i--);
	}
	EXPECT_EQ(i, TypeParam(-10));
}
