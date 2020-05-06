#include <math_lib/vec_base.h>

#include <math_lib/intersection.h>

#include <boost/qvm/vec_operations.hpp>

#include <gtest/gtest.h>

using namespace pagoda;

TEST(PlaneVsPlaneIntersectionTest, when_the_planes_intersect_should_return_a_line_intersection)
{
	Plane<float> p1(Vec3F{1, 0, 0}, 0);
	Plane<float> p2(Vec3F{0, 1, 0}, 0);

	auto i = intersection(p1, p2);
	EXPECT_EQ(i.m_type, PlaneIntersectionType::Type::Intersection);
	EXPECT_EQ(boost::qvm::dot(i.m_intersection.GetSupportVector(), p1.GetNormal()), 0);
	EXPECT_EQ(boost::qvm::dot(i.m_intersection.GetSupportVector(), p2.GetNormal()), 0);
}

TEST(PlaneVsPlaneIntersectionTest, when_the_planes_dont_intersect_should_return_no_intersection)
{
	Plane<float> p1(Vec3F{1, 0, 0}, 0);
	Plane<float> p2(Vec3F{1, 0, 0}, 1);

	auto i = intersection(p1, p2);
	EXPECT_EQ(i.m_type, PlaneIntersectionType::Type::NoIntersection);
}

TEST(PlaneVsPlaneIntersectionTest, when_the_planes_are_coplanar_should_return_coplanar_intersection)
{
	Plane<float> p1(Vec3F{1, 0, 0}, 0);
	Plane<float> p2(Vec3F{1, 0, 0}, 0);

	auto i = intersection(p1, p2);
	EXPECT_EQ(i.m_type, PlaneIntersectionType::Type::Coplanar);
}

TEST(PlaneVsLineIntersectionTest, when_the_plane_and_line_intersect_should_return_the_intersection_point)
{
	Plane<float> p(Vec3F{1, 0, 0}, 0);
	Line3D<float> l(Vec3F{0, 0, 0}, Vec3F{1, 0, 0});

	auto i = intersection(p, l);
	EXPECT_EQ(i.m_type, PlaneLineIntersection::Type::Intersection);
	EXPECT_TRUE(i.m_intersection == (Vec3F{0, 0, 0}));
}

TEST(PlaneVsLineIntersectionTest, when_the_plane_and_line_dont_intersect_should_return_no_intersection)
{
	Plane<float> p(Vec3F{1, 0, 0}, 0);
	Line3D<float> l(Vec3F{1, 0, 0}, Vec3F{0, 1, 0});

	auto i = intersection(p, l);
	EXPECT_EQ(i.m_type, PlaneLineIntersection::Type::NoIntersection);
}

TEST(PlaneVsLineIntersectionTest, when_the_plane_and_line_are_coplanar_should_return_coplanar)
{
	Plane<float> p(Vec3F{1, 0, 0}, 0);
	Line3D<float> l(Vec3F{0, 0, 0}, Vec3F{0, 1, 0});

	auto i = intersection(p, l);
	EXPECT_EQ(i.m_type, PlaneLineIntersection::Type::Coplanar);
}

TEST(PlaneVsLineIntersectionTest, has_intersection_test)
{
	// clang-format off
	std::vector<std::tuple<Plane<float>, Line3D<float>, Vec3F>> tests = {
	    {Plane<float>::FromPointAndNormal({0, 0, 0}, {1, 0, 0}), Line3D<float>::FromTwoPoints({-5, -5, 0}, {5, -5, 0}), Vec3F{0, -5, 0}}
    };
	// clang-format on

	for (const auto &t : tests)
	{
		auto i = intersection(std::get<0>(t), std::get<1>(t));
		EXPECT_TRUE(i.m_intersection == std::get<2>(t));
	}
}

