#include <math_lib/nearest_points.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(LineVsLineNearestPointsTest, when_lines_dont_interset_should_return_the_nearest_points_on_both)
{
	Line3<float> l1(Vec3F(0,0,0), Vec3F(1,0,0));
	Line3<float> l2(Vec3F(1,0,0), Vec3F(0,1,0));
	
	auto n = nearest_points(l1, l2);
	
	EXPECT_EQ(n.m_point1, Vec3F(0,0,0));
	EXPECT_EQ(n.m_point2, Vec3F(1,0,0));
}

TEST(LineVsLineNearestPointsTest, when_lines_are_colinear_should_return_the_same_point)
{
	Line3<float> l1(Vec3F(0,0,0), Vec3F(1,0,0));
	Line3<float> l2(Vec3F(0,0,0), Vec3F(1,0,0));
	
	auto n = nearest_poitns(l1, l2);
	
	EXPECT_EQ(n.m_point1, n.m_point2);
}

TEST(LineVsLineSegmentNearestPoints, when_they_dont_intersect_should_return_the_nearest_points_on_both)
{
	Line3<float> l1(Vec3F(0,0,0), Vec3F(1,0,0));
	LineSegment3<float> l2(Vec3F(1,0,0), Vec3F(2,0,0));
	
	auto n = nearest_points(l1, l2);
	
	EXPECT_EQ(n.m_point1, Vec3F(0,0,0));
	EXPECT_EQ(n.m_point2, Vec3F(1,0,0));
	
	n = nearest_points(l2, l1);
	EXPECT_EQ(n.m_point2, Vec3F(0,0,0));
	EXPECT_EQ(n.m_point1, Vec3F(1,0,0));
}

TEST(LineVsLineSegmentNearestPoints, when_they_are_colinear_should_return_the_same_point)
{
	Line3<float> l1(Vec3F(0,0,0), Vec3F(1,0,0));
	LineSegment3<float> l2(Vec3F(0,0,0), Vec3F(1,0,0));
	
	auto n = nearest_points(l1, l2);
	EXPECT_EQ(n.m_point1, n.m_point2);
}

