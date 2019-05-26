#include <geometry_core/geometry.h>
#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_topology.h>
#include <math_lib/vec_base.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../test_utils.h"
#include "mock_objects.h"

using namespace selector;
using namespace ::testing;

using GeometryType = GeometryT<SplitPointTopology<uint32_t>>;

class GeometryBuilderTest : public ::testing::Test
{
protected:
	virtual void SetUp() { m_mockGeometry = std::make_shared<MockGeometry>(); }

	virtual void TearDown() {}

	std::shared_ptr<MockGeometry> m_mockGeometry;
};

TEST_F(GeometryBuilderTest, when_creating_points_should_delegate_creation_to_geometry)
{
	// clang-format off
    EXPECT_CALL(*m_mockGeometry, CreateVertex(Vec3F{0,0,0}))
        .Times(1)
        .WillOnce(Return(0));
    EXPECT_CALL(*m_mockGeometry, CreateVertex(Vec3F{0,0,1}))
        .Times(1)
        .WillOnce(Return(1));
	// clang-format on

	auto builder = std::make_shared<GeometryBuilderT<MockGeometry>>(m_mockGeometry);
	builder->AddPoint({0, 0, 0});
	builder->AddPoint({0, 0, 1});
}

TEST_F(GeometryBuilderTest, when_creating_points_should_return_index_returned_by_geometry)
{
	// clang-format off
    EXPECT_CALL(*m_mockGeometry, CreateVertex(Vec3F{1,2,3}))
        .Times(1)
        .WillOnce(Return(123));
	// clang-format on

	auto builder = std::make_shared<GeometryBuilderT<MockGeometry>>(m_mockGeometry);
	EXPECT_EQ(builder->AddPoint(Vec3F{1, 2, 3}), 123);
}

TEST_F(GeometryBuilderTest, when_starting_a_single_face_should_return_an_initialized_face_builder)
{
	// clang-format off
    EXPECT_CALL(*m_mockGeometry, GetReservedFaces())
        .Times(1)
        .WillOnce(Return(1));
	// clang-format on
	//
	auto builder = std::make_shared<GeometryBuilderT<MockGeometry>>(m_mockGeometry);
	FaceBuilder<MockGeometry> faceBuilder = builder->StartFace(4);
	EXPECT_EQ(faceBuilder.m_geometry, m_mockGeometry);
	EXPECT_EQ(faceBuilder.m_faceIndices.size(), 0);
	EXPECT_EQ(faceBuilder.m_faceIndices.capacity(), 4);
}

TEST_F(GeometryBuilderTest, when_starting_many_faces_with_same_size_return_same_number_of_face_builders)
{
	// clang-format off
    EXPECT_CALL(*m_mockGeometry, GetReservedFaces())
        .Times(1)
        .WillOnce(Return(10));
	// clang-format on
	auto builder = std::make_shared<GeometryBuilderT<MockGeometry>>(m_mockGeometry);
	std::vector<FaceBuilder<MockGeometry>> faceBuilders = builder->StartFaces(10, 3);

	EXPECT_EQ(faceBuilders.size(), 10);
	for (const auto &fb : faceBuilders)
	{
		EXPECT_EQ(fb.m_geometry, m_mockGeometry);
		EXPECT_EQ(fb.m_faceIndices.size(), 0);
		EXPECT_EQ(fb.m_faceIndices.capacity(), 3);
	}
}

TEST_F(GeometryBuilderTest, when_starting_many_faces_with_different_size_return_same_number_of_face_builders)
{
	// clang-format off
    EXPECT_CALL(*m_mockGeometry, GetReservedFaces())
        .Times(1)
        .WillOnce(Return(3));
	// clang-format on
	auto builder = std::make_shared<GeometryBuilderT<MockGeometry>>(m_mockGeometry);
	std::vector<FaceBuilder<MockGeometry>> faceBuilders = builder->StartFaces(std::vector<uint32_t>{1, 2, 3});

	EXPECT_EQ(faceBuilders.size(), 3);
	uint32_t size = 1;
	for (const auto &fb : faceBuilders)
	{
		EXPECT_EQ(fb.m_geometry, m_mockGeometry);
		EXPECT_EQ(fb.m_faceIndices.size(), 0);
		EXPECT_EQ(fb.m_faceIndices.capacity(), size++);
	}
}

class FaceBuilderTest : public Test
{
protected:
	virtual void SetUp() { m_mockGeometry = std::make_shared<MockGeometry>(); }

	virtual void TearDown() {}

	std::shared_ptr<MockGeometry> m_mockGeometry;
};

TEST_F(FaceBuilderTest, when_creating_should_reserve_size_for_indices)
{
	FaceBuilder<MockGeometry> faceBuilder(this->m_mockGeometry, 10);

	EXPECT_EQ(faceBuilder.m_geometry, m_mockGeometry);
	EXPECT_EQ(faceBuilder.m_faceIndices.size(), 0);
	EXPECT_EQ(faceBuilder.m_faceIndices.capacity(), 10);
}

TEST_F(FaceBuilderTest, when_adding_indices_should_add_in_order)
{
	FaceBuilder<MockGeometry> faceBuilder(m_mockGeometry, 3);

	faceBuilder.AddIndex(1);
	faceBuilder.AddIndex(2);
	faceBuilder.AddIndex(3);

	EXPECT_EQ(faceBuilder.m_faceIndices.size(), 3);
	EXPECT_EQ(faceBuilder.m_faceIndices[0], 1);
	EXPECT_EQ(faceBuilder.m_faceIndices[1], 2);
	EXPECT_EQ(faceBuilder.m_faceIndices[2], 3);
}

TEST_F(FaceBuilderTest, when_closing_face_should_delegate_to_geometry_should_return_same_face_index)
{
	// clang-format off
    EXPECT_CALL(*m_mockGeometry, UntemplatedCreateFace(std::vector<uint32_t>{1,2,3}))
            .Times(1)
            .WillOnce(Return(123));
	// clang-format on
	FaceBuilder<MockGeometry> faceBuilder(m_mockGeometry, 3);

	faceBuilder.AddIndex(1);
	faceBuilder.AddIndex(2);
	faceBuilder.AddIndex(3);

	EXPECT_EQ(faceBuilder.CloseFace(), 123);
}
