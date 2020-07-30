#include <pagoda/geometry/algorithms/traverse.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/math/vec_base.h>
#include <memory>

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>

using namespace pagoda;
using namespace pagoda::geometry;
using namespace pagoda::math;

class TraverseTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_geometry = std::make_shared<core::Geometry>();
		core::GeometryBuilder builder(m_geometry);

		// clang-format off
		core::Geometry::Index_t points[] = {
            builder.AddPoint({0, 0, 0}),        // 0
            builder.AddPoint({1, 0, 0}),        // 1
            builder.AddPoint({1, 1, 0}),        // 2
            builder.AddPoint({0, 1, 0}),        // 3

            builder.AddPoint({0, 0, 1}),        // 4
            builder.AddPoint({1, 0, 1}),        // 5
            builder.AddPoint({1, 1, 1}),        // 6
            builder.AddPoint({0, 1, 1}),        // 7
		};

        core::Geometry::Index_t faces[6][4] = {
            {0, 3, 2, 1},
            {0, 1, 5, 4},
            {1, 2, 6, 5},
            {2, 3, 7, 6},
            {3, 0, 4, 7},
            {4, 5, 6, 7}
        };
		// clang-format on

		for (auto f = 0u; f < 6; ++f)
		{
			auto faceBuilder = builder.StartFace(4);
			for (auto fp = 0u; fp < 4; ++fp)
			{
				faceBuilder.AddIndex(points[faces[f][fp]]);
			}
			faceBuilder.CloseFace();
		}
	}

	core::GeometryPtr m_geometry;
};

TEST_F(TraverseTest, should_traverse_all_faces)
{
	uint32_t count = 0;
	algorithms::EachFace<core::Geometry>(m_geometry.get(),
	                                     [&count](core::Geometry *g, const core::Geometry::FaceHandle &f) {
		                                     EXPECT_EQ(f.GetIndex(), count);
		                                     ++count;
	                                     });
}

TEST_F(TraverseTest, should_traverse_all_points)
{
	std::vector<uint32_t> expected({0, 1, 2, 3, 4, 5, 6, 7});

	uint32_t index = 0;
	algorithms::EachPoint<core::Geometry>(m_geometry.get(),
	                                      [&index, &expected](core::Geometry *g, const core::Geometry::PointHandle &p) {
		                                      EXPECT_EQ(p.GetIndex(), expected[index++]);
	                                      });
}

TEST_F(TraverseTest, should_traverse_all_edges)
{
	std::vector<uint32_t> expected(
	    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23});

	uint32_t index = 0;
	algorithms::EachEdge<core::Geometry>(m_geometry.get(),
	                                     [&index, &expected](core::Geometry *g, const core::Geometry::EdgeHandle &e) {
		                                     EXPECT_EQ(e.GetIndex(), expected[index++]);
	                                     });
}

TEST_F(TraverseTest, should_traverse_all_split_points)
{
	std::vector<uint32_t> expected(
	    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23});

	uint32_t index = 0;
	algorithms::EachSplitPoint<core::Geometry>(
	    m_geometry.get(), [&index, &expected](core::Geometry *g, const core::Geometry::SplitPointHandle &s) {
		    EXPECT_EQ(s.GetIndex(), expected[index++]);
	    });
}

TEST_F(TraverseTest, should_circulate_all_points_around_each_face)
{
	std::vector<uint32_t> expected({0, 1, 2, 3, 0, 3, 4, 5, 3, 2, 6, 4, 2, 1, 7, 6, 1, 0, 5, 7, 5, 4, 6, 7});

	uint32_t index = 0;
	algorithms::EachPointAroundEachFace<core::Geometry>(
	    m_geometry.get(),
	    [&index, &expected](core::Geometry *g, const core::Geometry::FaceHandle &f,
	                        const core::Geometry::PointHandle &p) { EXPECT_EQ(p.GetIndex(), expected[index++]); });
}

TEST_F(TraverseTest, should_circulate_all_points_around_a_face)
{
	std::vector<uint32_t> expected({0, 1, 2, 3, 0, 3, 4, 5, 3, 2, 6, 4, 2, 1, 7, 6, 1, 0, 5, 7, 5, 4, 6, 7});

	uint32_t index = 0;
	algorithms::EachFace<core::Geometry>(
	    m_geometry.get(), [&index, &expected](core::Geometry *g, const core::Geometry::FaceHandle &f) {
		    algorithms::EachPointAroundFace(
		        g, f, [&index, &expected](core::Geometry *, const core::Geometry::PointHandle &p) {
			        EXPECT_EQ(p.GetIndex(), expected[index++]);
		        });
	    });
}

TEST_F(TraverseTest, should_circulate_all_split_points_around_a_face)
{
	std::vector<uint32_t> expected(
	    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23});

	uint32_t index = 0;
	algorithms::EachFace<core::Geometry>(
	    m_geometry.get(), [&index, &expected](core::Geometry *g, const core::Geometry::FaceHandle &f) {
		    algorithms::EachSplitPointAroundFace(
		        g, f, [&index, &expected](core::Geometry *, const core::Geometry::SplitPointHandle &p) {
			        EXPECT_EQ(p.GetIndex(), expected[index++]);
		        });
	    });
}
