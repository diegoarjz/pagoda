#include <math_lib/line_3d.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(Line3D, when_using_default_constructor_should_create_the_x_axis)
{
    Line3D<float> l;
    ASSERT_EQ(l.GetPoint(), Vec3F(0,0,0));
    ASSERT_EQ(l.GetDirection(), Vec3F(1,0,0));
}

TEST(Line3D, when_using_point_direction_constructor_should_create_the_respective_line)
{
    Line3D<float> l(Vec3F(1,2,3), Vec3F(0,1,0));
    ASSERT_EQ(l.GetPoint(), Vec3F(1,2,3));
    ASSERT_EQ(l.GetDirection(), Vec3F(0,1,0));
}

TEST(Line3D, when_using_two_points_static_constructor_should_create_the_respective_line)
{
    Line3D<float> l = Line3D<float>::FromTwoPoints(Vec3F(1,2,3), Vec3F(3,2,3));
    ASSERT_EQ(l.GetPoint(), Vec3F(1,2,3));
    ASSERT_EQ(l.GetDirection(), Vec3F(1,0,0));
}