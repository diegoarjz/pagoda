#include <dynamic_value/dynamic_plane.h>
#include <dynamic_value/get_value_as.h>
#include <dynamic_value/set_value_from.h>
#include <dynamic_value/type_info.h>
#include <dynamic_value/vector3.h>

#include <math_lib/plane.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace selector;

TEST(DynamicPlaneTest, test_construction)
{
	DynamicPlane p;
	EXPECT_EQ(Plane<float>(), static_cast<Plane<float>>(p));

	DynamicPlane p2(Plane<float>::FromPointAndNormal(Vec3F(0, 0, 0), Vec3F(1, 1, 1)));
	EXPECT_EQ(p2, Plane<float>::FromPointAndNormal(Vec3F(0, 0, 0), Vec3F(1, 1, 1)));

	DynamicPlane p3(std::make_shared<Vector3>(Vec3F(0, 0, 0)), std::make_shared<Vector3>(Vec3F(1, 1, 1)));
	EXPECT_EQ(p3, Plane<float>::FromPointAndNormal(Vec3F(0, 0, 0), Vec3F(1, 1, 1)));
}

TEST(DynamicPlaneTest, test_get_value_as)
{
	auto nativePlane = Plane<float>::FromPointAndNormal(Vec3F(0, 0, 0), Vec3F(1, 1, 1));
	DynamicPlane p(nativePlane);
	EXPECT_EQ(static_cast<Plane<float>>(p), nativePlane);
}

TEST(DynamicPlaneTest, test_set_value_from)
{
	auto nativePlane = Plane<float>::FromPointAndNormal(Vec3F(0, 0, 0), Vec3F(1, 1, 1));
	DynamicPlane p;
	set_value_from<Plane<float>>(p, nativePlane);
	EXPECT_EQ(static_cast<Plane<float>>(p), nativePlane);
}
