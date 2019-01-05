#include <geometry_core/geometry.h>
#include <geometry_core/geometry_topology.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>
#include <algorithm>

#include "../test_utils.h"

using namespace selector;

template<class Topology>
class GeometryTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		empty_geometry = std::make_shared<GeometryT<Topology>>(3, 3, 1);
		invalidIndex = GeometryT<Topology>::sInvalidIndex;
	}

	virtual void TearDown() {}

	std::shared_ptr<GeometryT<Topology>> empty_geometry;

	typename Topology::IndexType invalidIndex;
};

typedef ::testing::Types<IndexedFacesTopology<uint32_t>, SplitPointTopology<uint32_t>> TopologyTypes;
TYPED_TEST_CASE(GeometryTest, TopologyTypes);

TYPED_TEST(GeometryTest, when_created_should_be_empty)
{
	EXPECT_EQ(this->empty_geometry->GetNumFaces(), 0);
	EXPECT_EQ(this->empty_geometry->GetNumEdges(), 0);
	EXPECT_EQ(this->empty_geometry->GetNumVertices(), 0);
}

TYPED_TEST(GeometryTest, when_created_should_have_space_reserved)
{
	EXPECT_EQ(this->empty_geometry->GetReservedFaces(), 1);
	EXPECT_EQ(this->empty_geometry->GetReservedEdges(), 3);
	EXPECT_EQ(this->empty_geometry->GetReservedVertices(), 3);
}

TYPED_TEST(GeometryTest, when_creating_vertices_should_return_incrementing_vertex_indices)
{
	EXPECT_EQ(this->empty_geometry->CreateVertex({0, 0, 0}), 0);
	EXPECT_EQ(this->empty_geometry->CreateVertex({0, 0, 1}), 1);
	EXPECT_EQ(this->empty_geometry->CreateVertex({0, 0, 1}), 2);
}

TYPED_TEST(GeometryTest, when_creating_more_vertices_than_reserved_should_return_invalid_index)
{
	this->empty_geometry->CreateVertex({0, 0, 0});
	this->empty_geometry->CreateVertex({0, 0, 0});
	this->empty_geometry->CreateVertex({0, 0, 0});
	EXPECT_EQ(this->empty_geometry->CreateVertex({0, 0, 0}), this->invalidIndex);
}

TYPED_TEST(GeometryTest, when_creating_more_faces_than_reserved_should_return_invalid_index)
{
	std::vector<typename TypeParam::IndexType> triangle_indices = {this->empty_geometry->CreateVertex({0, 0, 0}),
	                                                               this->empty_geometry->CreateVertex({0, 0, 0}),
	                                                               this->empty_geometry->CreateVertex({0, 0, 0})};

	this->empty_geometry->CreateFace(triangle_indices);
	EXPECT_EQ(this->empty_geometry->CreateFace(triangle_indices), this->invalidIndex);
}

TYPED_TEST(GeometryTest, when_creating_faces_should_set_the_face_normal)
{
	std::vector<Vec3F> triangle_vertices = {{0, 0, 0}, {1, 0, 0}, {1, 1, 0}};
	std::vector<typename TypeParam::IndexType> triangle_indices = {
	    this->empty_geometry->CreateVertex(triangle_vertices[0]),
	    this->empty_geometry->CreateVertex(triangle_vertices[1]),
	    this->empty_geometry->CreateVertex(triangle_vertices[2]),
	};

	auto face_index = this->empty_geometry->CreateFace(triangle_indices);
	auto normal = face_normal(this->empty_geometry, face_index);

	Vec3F expected(0, 0, 1);

	EXPECT_EQ(normal, expected);
}
