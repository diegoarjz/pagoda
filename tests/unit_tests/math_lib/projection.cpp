#include <pagoda/math_lib/vec_base.h>

#include <pagoda/math_lib/projection.h>

#include <gtest/gtest.h>

using namespace pagoda;

TEST(ProjectionTest, when_projecting_a_point_to_a_line_should_return_the_correct_point)
{
	Vec3F p{1, 1, 1};
	Line3D<float> l(Vec3F{0, 0, 0}, Vec3F{0, 1, 0});
	EXPECT_TRUE(projection(p, l) == (Vec3F{0, 1, 0}));
}

TEST(ProjectionTest, when_projecting_a_point_to_a_line_segment_inside_the_endpoints_should_return_the_projected_point)
{
	LineSegment3D<float> ls(Vec3F{0, 1, 0}, Vec3F{1, 1, 0});
	Vec3F p{0.5, 0, 0};
	ASSERT_TRUE(projection(p, ls) == (Vec3F{0.5, 1, 0}));
}

TEST(ProjectionTest, when_projecting_a_point_to_a_line_segment_before_the_source_point_should_return_the_source_point)
{
	LineSegment3D<float> ls(Vec3F{0, 1, 0}, Vec3F{1, 1, 0});
	Vec3F p{-1, 0, 0};
	ASSERT_TRUE(projection(p, ls) == (Vec3F{0, 1, 0}));
}

TEST(ProjectionTest, when_projecting_a_point_to_a_line_segment_after_the_target_point_should_return_the_target_point)
{
	LineSegment3D<float> ls(Vec3F{0, 1, 0}, Vec3F{1, 1, 0});
	Vec3F p{2, 0, 0};
	ASSERT_TRUE(projection(p, ls) == (Vec3F{1, 1, 0}));
}

TEST(ProjectionTest, when_projecting_a_line_segment_to_a_line_should_return_a_line_segment_with_the_projected_points)
{
	LineSegment3D<float> ls(Vec3F{0, 1, 0}, Vec3F{1, 1, 0});
	Line3D<float> l(Vec3F{0, 0, 0}, Vec3F{1, 0, 0});
	ASSERT_TRUE(projection(ls, l) == LineSegment3D<float>((Vec3F{0, 0, 0}), (Vec3F{1, 0, 0})));
}

TEST(ProjectionTest,
     when_projecting_a_line_segment_that_is_orthogonal_to_a_line_should_return_a_degenerated_line_segment)
{
	LineSegment3D<float> ls(Vec3F{0, 0, 0}, Vec3F{0, 1, 0});
	Line3D<float> l(Vec3F{0, 0, 0}, Vec3F{1, 0, 0});
	ASSERT_TRUE(projection(ls, l) == LineSegment3D<float>((Vec3F{0, 0, 0}), (Vec3F{0, 0, 0})));
}

TEST(ProjectionTest, when_projecting_a_point_onto_a_plane_should_return_the_projected_point)
{
	Plane<float> plane(Vec3F{0, 0, 1}, 10);
	Vec3F p{1, 3, 0};
	EXPECT_TRUE(projection(p, plane) == (Vec3F{1, 3, 10}));
}
