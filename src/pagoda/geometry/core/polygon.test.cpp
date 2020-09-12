#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/geometry/core/polygon.h>
#include <array>

#include <gtest/gtest.h>

#include <iostream>

using namespace pagoda;
using namespace pagoda::math;
using namespace pagoda::geometry::core;
using namespace ::testing;

TEST(PolygonTest, should_be_constructable)
{
	Polygon3 p({{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}});
	EXPECT_EQ(p.GetPointCount(), 4);
}

TEST(PolygonTest, should_be_able_to_transform_the_points)
{
	Polygon3 p({{0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}});
	p.Transform([](const Vec3F& p) { return p + Vec3F{1, 0, 0}; });
	std::vector<Vec3F> expected{{1, 0, 0}, {2, 0, 0}, {2, 1, 0}, {1, 1, 0}};
	uint32_t i = 0;
	for (const auto& point : p.GetPoints()) {
		EXPECT_TRUE(point == expected[i++]);
	}
}

TEST(PolygonTest, should_be_comparable)
{
	Polygon2 p1({{0, 0}, {1, 0}, {1, 1}});
	Polygon2 p2({{0, 0}, {1, 0}, {1, 1}});
	Polygon2 p3({{0, 0}, {1, 0}, {0, 1}});

	EXPECT_TRUE(p1 == p2);
	EXPECT_TRUE(p1 != p3);
}
