#include "planar_texture_projection.h"

#include <gtest/gtest.h>

#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <iostream>

using namespace boost::qvm;
using namespace pagoda::geometry::core;

TEST(PlanarTextureProjectionTest, well_known_coordinates)
{
	PlanarTextureProjection p({0, 0, 0}, {1, 0, 0}, {0, 1, 0});

	EXPECT_TRUE((p.GetProjection({0, 0, 0}) == vec<float, 2>{0, 0}));
	EXPECT_TRUE((p.GetProjection({1, 0, 0}) == vec<float, 2>{1, 0}));
	EXPECT_TRUE((p.GetProjection({0, 1, 0}) == vec<float, 2>{0, 1}));
	EXPECT_TRUE((p.GetProjection({1, 1, 0}) == vec<float, 2>{1, 1}));
	EXPECT_TRUE((p.GetProjection({0.5, 0.5, 0}) == vec<float, 2>{0.5, 0.5}));
}

TEST(PlanarTextureProjectionTest, offset)
{
	PlanarTextureProjection p({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
	p.SetOffset(0.5f, 0.0f);

	EXPECT_TRUE((p.GetProjection({0, 0, 0}) == vec<float, 2>{-0.5f, 0.0f}));

	p.SetOffset(0.0f, 0.4f);
	EXPECT_TRUE((p.GetProjection({0, 0, 0}) == vec<float, 2>{0.0f, -0.4f}));
}

TEST(PlanarTextureProjectionTest, scale)
{
	PlanarTextureProjection p({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
	p.SetScale(2, 1);

	EXPECT_TRUE((p.GetProjection({0, 0, 0}) == vec<float, 2>{0, 0}));
	EXPECT_TRUE((p.GetProjection({1, 0, 0}) == vec<float, 2>{0.5, 0}));

	p.SetScale(1, 2);

	EXPECT_TRUE((p.GetProjection({0, 0, 0}) == vec<float, 2>{0, 0}));
	EXPECT_TRUE((p.GetProjection({1, 0, 0}) == vec<float, 2>{1, 0}));
	EXPECT_TRUE((p.GetProjection({0, 1, 0}) == vec<float, 2>{0, 0.5f}));
}

TEST(PlanarTextureProjectionTest, clamp)
{
	PlanarTextureProjection p({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
	p.SetClamp(true);

	EXPECT_TRUE((p.GetProjection({2, 0, 0}) == vec<float, 2>{1, 0}));
	EXPECT_TRUE((p.GetProjection({-1, 0, 0}) == vec<float, 2>{0, 0}));
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	auto returnVal = RUN_ALL_TESTS();
	return returnVal;
}
