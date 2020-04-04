#include <math_lib/vec_base.h>

#include <math_lib/length.h>
#include <math_lib/line_segment_3d.h>
#include <boost/qvm/vec_operations.hpp>

#include <gtest/gtest.h>

using namespace selector;

TEST(LineSegment3D, when_using_default_constructor_should_create_a_unit_line_segment_in_the_x_axis)
{
	LineSegment3D<float> l;
	ASSERT_TRUE(l.GetSourcePoint() == (Vec3F{0, 0, 0}));
	ASSERT_TRUE(l.GetTargetPoint() == (Vec3F{1, 0, 0}));
}

TEST(LineSegment3D, when_using_two_points_constructor_should_create_the_respective_line_segment)
{
	LineSegment3D<float> l(Vec3F{0, 0, 0}, Vec3F{1, 2, 3});
	ASSERT_TRUE(l.GetSourcePoint() == (Vec3F{0, 0, 0}));
	ASSERT_TRUE(l.GetTargetPoint() == (Vec3F{1, 2, 3}));
}

TEST(LineSegment3D, when_comparing_two_equal_line_segments_should_return_equal)
{
	LineSegment3D<float> l1;
	LineSegment3D<float> l2(Vec3F{0, 0, 0}, Vec3F{1, 0, 0});
	ASSERT_TRUE(l1 == l2);
	ASSERT_FALSE(l1 != l2);
}

TEST(LineSegment3D, when_comparing_two_different_line_segments_should_return_not_equal)
{
	LineSegment3D<float> l1(Vec3F{0, 0, 0}, Vec3F{1, 2, 3});
	LineSegment3D<float> l2(Vec3F{0, 0, 0}, Vec3F{1, 2, 4});
	ASSERT_FALSE(l1 == l2);
	ASSERT_TRUE(l1 != l2);
}

