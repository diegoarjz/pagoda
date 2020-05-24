#include <pagoda/math/nearest_points.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::math;

TEST(LineVsLineNearestPointsTest, when_lines_dont_interset_should_return_the_nearest_points_on_both)
{
	Line3D<float> l1(boost::qvm::vec<float, 3>{0, 0, 0}, boost::qvm::vec<float, 3>{1, 0, 0});
	Line3D<float> l2(boost::qvm::vec<float, 3>{1, 0, 1}, boost::qvm::vec<float, 3>{0, 1, 0});

	auto n = nearest_points(l1, l2);

	EXPECT_TRUE(n.m_point1 == (boost::qvm::vec<float, 3>{1, 0, 0}));
	EXPECT_TRUE(n.m_point2 == (boost::qvm::vec<float, 3>{1, 0, 1}));
}

TEST(LineVsLineNearestPointsTest, when_lines_are_colinear_should_return_the_same_point)
{
	Line3D<float> l1(boost::qvm::vec<float, 3>{0, 0, 0}, boost::qvm::vec<float, 3>{1, 0, 0});
	Line3D<float> l2(boost::qvm::vec<float, 3>{0, 0, 0}, boost::qvm::vec<float, 3>{1, 0, 0});

	auto n = nearest_points(l1, l2);

	EXPECT_TRUE(n.m_point1 == n.m_point2);
}

TEST(LineVsLineSegmentNearestPoints, when_they_dont_intersect_should_return_the_nearest_points_on_both)
{
	Line3D<float> l1(boost::qvm::vec<float, 3>{0, 0, 0}, boost::qvm::vec<float, 3>{1, 0, 0});
	LineSegment3D<float> l2(boost::qvm::vec<float, 3>{1, 0, 1}, boost::qvm::vec<float, 3>{1, 1, 1});

	auto n = nearest_points(l1, l2);

	EXPECT_TRUE(n.m_point1 == (boost::qvm::vec<float, 3>{1, 0, 0}));
	EXPECT_TRUE(n.m_point2 == (boost::qvm::vec<float, 3>{1, 0, 1}));

	n = nearest_points(l2, l1);
	EXPECT_TRUE(n.m_point1 == (boost::qvm::vec<float, 3>{1, 0, 1}));
	EXPECT_TRUE(n.m_point2 == (boost::qvm::vec<float, 3>{1, 0, 0}));
}

TEST(LineVsLineSegmentNearestPoints, when_they_are_colinear_should_return_the_same_point)
{
	Line3D<float> l1(boost::qvm::vec<float, 3>{0, 0, 0}, boost::qvm::vec<float, 3>{1, 0, 0});
	LineSegment3D<float> l2(boost::qvm::vec<float, 3>{0, 0, 0}, boost::qvm::vec<float, 3>{1, 0, 0});

	auto n = nearest_points(l1, l2);
	EXPECT_TRUE(n.m_point1 == n.m_point2);
}

