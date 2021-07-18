#include "utils.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace pagoda;
using namespace pagoda::common;

TEST(CartesianProductTest, test_empty)
{
	std::vector<std::vector<int>> values{{}, {}};
	auto count = CartesianProduct(values, [](const std::vector<int>& v) {});
	EXPECT_EQ(count, 0);
}

TEST(CartesianProductTest, test_one_empty)
{
	std::vector<std::vector<int>> values{{1}, {}};
	auto count = CartesianProduct(values, [](const std::vector<int>& v) {});
	EXPECT_EQ(count, 0);
}

TEST(CartesianProductTest, test_2x2)
{
	std::vector<std::vector<int>> values{{0, 1}, {2, 3}};
	std::vector<std::vector<int>> expected{{0, 2}, {1, 2}, {0, 3}, {1, 3}};
	std::size_t i = 0;
	auto count = CartesianProduct(
	  values, [&](const std::vector<int>& v) { EXPECT_EQ(v, expected[i++]); });

	EXPECT_EQ(count, expected.size());
}

TEST(CartesianProductTest, test_different_sizes)
{
	std::vector<std::vector<int>> values{{0, 1}, {2, 3}, {0, 1, 2}};
	std::vector<std::vector<int>> expected{
	  {0, 2, 0}, {1, 2, 0}, {0, 3, 0}, {1, 3, 0},

	  {0, 2, 1}, {1, 2, 1}, {0, 3, 1}, {1, 3, 1},

	  {0, 2, 2}, {1, 2, 2}, {0, 3, 2}, {1, 3, 2},
	};
	std::size_t i = 0;
	auto count = CartesianProduct(
	  values, [&](const std::vector<int>& v) { EXPECT_EQ(v, expected[i++]); });

	EXPECT_EQ(count, expected.size());
}
