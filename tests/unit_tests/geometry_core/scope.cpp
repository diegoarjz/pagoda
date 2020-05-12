#include <pagoda/geometry_core/geometry.h>
#include <pagoda/geometry_core/geometry_builder.h>
#include <pagoda/geometry_core/scope.h>

#include <gtest/gtest.h>

#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

using namespace pagoda;
using GeometryType = GeometryBase<>;

TEST(Scope, test_scope_construction)
{
	Scope s;

	EXPECT_TRUE(s.GetPosition() == (Vec3F{0, 0, 0}));
	EXPECT_TRUE(s.GetSize() == (Vec3F{0, 0, 0}));
	EXPECT_TRUE(boost::qvm::col<0>(s.GetRotation()) == (Vec3F{1, 0, 0}));
	EXPECT_TRUE(boost::qvm::col<1>(s.GetRotation()) == (Vec3F{0, 1, 0}));
	EXPECT_TRUE(boost::qvm::col<2>(s.GetRotation()) == (Vec3F{0, 0, 1}));
}

TEST(Scope, when_constructing_from_box_points_should_create_a_correct_scope)
{
	// clang-format off
	std::array<Vec3F, 8> boxPoints = {
	    Vec3F{0, 0, 0}, Vec3F{0, 2, 0}, Vec3F{-1, 0, 0}, Vec3F{-1, 1, 0},
        Vec3F{0, 0, 3}, Vec3F{0, 2, 3}, Vec3F{-1, 0, 3}, Vec3F{-1, 1, 3},
	};
	// clang-format on

	Scope s(boxPoints);

	EXPECT_TRUE(s.GetPosition() == (Vec3F{0, 0, 0}));
	EXPECT_TRUE(s.GetSize() == (Vec3F{2, 1, 3}));
	EXPECT_TRUE(boost::qvm::col<0>(s.GetRotation()) == (Vec3F{0, 1, 0}));
	EXPECT_TRUE(boost::qvm::col<1>(s.GetRotation()) == (Vec3F{-1, 0, 0}));
	EXPECT_TRUE(boost::qvm::col<2>(s.GetRotation()) == (Vec3F{0, 0, 1}));
}

TEST(Scope, when_constructing_from_geometry_and_constrained_rotation_should_create_a_correct_scope)
{
	auto geometry = std::make_shared<GeometryType>();
	GeometryBuilderT<GeometryType> builder(geometry);

	// Bottom face
	builder.AddPoint(Vec3F{0, 0, 0});
	builder.AddPoint(Vec3F{1, 0, 0});
	builder.AddPoint(Vec3F{1, 1, 0});
	builder.AddPoint(Vec3F{0, 1, 0});

	// Top face
	builder.AddPoint(Vec3F{0, 0, 1});
	builder.AddPoint(Vec3F{1, 0, 1});
	builder.AddPoint(Vec3F{1, 1, 1});
	builder.AddPoint(Vec3F{0, 1, 1});

	auto bottomFace = builder.StartFace(4);
	auto topFace = builder.StartFace(4);
	typename GeometryBuilderT<GeometryType>::FaceBuilder sideFaces[] = {
	    builder.StartFace(4),
	    builder.StartFace(4),
	    builder.StartFace(4),
	    builder.StartFace(4),
	};

	for (auto i : {3, 2, 1, 0})
	{
		bottomFace.AddIndex(i);
	}
	for (auto i : {4, 5, 6, 7})
	{
		topFace.AddIndex(i);
	}
	bottomFace.CloseFace();
	topFace.CloseFace();

	uint32_t i = 0;
	std::vector<std::vector<uint32_t>> sideFaceIndices;
	sideFaceIndices.push_back({0, 1, 5, 4});
	sideFaceIndices.push_back({1, 2, 6, 5});
	sideFaceIndices.push_back({2, 3, 7, 6});
	sideFaceIndices.push_back({3, 0, 4, 7});
	for (auto &f : sideFaceIndices)
	{
		for (auto index : f)
		{
			sideFaces[i].AddIndex(index);
		}
		sideFaces[i++].CloseFace();
	}

	auto s = Scope::FromGeometryAndConstrainedRotation(geometry, boost::qvm::diag_mat(XYZ(Vec3F{1, 1, 1})));
	EXPECT_TRUE(s.GetPosition() == (Vec3F{0, 0, 0}));
	EXPECT_TRUE(s.GetSize() == (Vec3F{1, 1, 1}));
	EXPECT_TRUE(boost::qvm::col<0>(s.GetRotation()) == (Vec3F{1, 0, 0}));
	EXPECT_TRUE(boost::qvm::col<1>(s.GetRotation()) == (Vec3F{0, 1, 0}));
	EXPECT_TRUE(boost::qvm::col<2>(s.GetRotation()) == (Vec3F{0, 0, 1}));
}
