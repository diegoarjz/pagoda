#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/math/vec_base.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../test_utils.h"
#include "mock_objects.h"

using namespace pagoda;
using namespace pagoda::math;
using namespace pagoda::geometry::core;
using namespace ::testing;

using GeometryType = GeometryBase<>;

class GeometryBuilderTest : public ::testing::Test
{
protected:
	virtual void SetUp() { m_geometry = std::make_shared<GeometryType>(); }

	virtual void TearDown() {}

	std::shared_ptr<GeometryType> m_geometry;
};

TEST_F(GeometryBuilderTest, when_creating_a_face_should_create_a_valid_geometry)
{
	auto builder = std::make_shared<GeometryBuilderT<GeometryType>>(m_geometry);
	auto p1 = builder->AddPoint({0, 0, 0});
	auto p2 = builder->AddPoint({1, 0, 0});
	auto p3 = builder->AddPoint({1, 1, 0});
	auto p4 = builder->AddPoint({0.5f, 0.5f, 0});
	auto p5 = builder->AddPoint({0, 1, 0});

	auto faceBuilder = builder->StartFace(5);
	faceBuilder.AddIndex(p1);
	faceBuilder.AddIndex(p2);
	faceBuilder.AddIndex(p3);
	faceBuilder.AddIndex(p4);
	faceBuilder.AddIndex(p5);
	faceBuilder.CloseFace();

	EXPECT_EQ(m_geometry->GetFaceCount(), 1);
	EXPECT_EQ(m_geometry->GetPointCount(), 5);
	EXPECT_EQ(m_geometry->GetSplitPointCount(), 5);
	EXPECT_EQ(m_geometry->GetEdgeCount(), 5);
	EXPECT_TRUE(m_geometry->IsValid());

	typename SplitPointTopology::SplitPointHandle currentSplitPoint = GeometryType::s_invalidIndex;
	for (auto iter = m_geometry->SplitPointsBegin(); iter != m_geometry->SplitPointsEnd(); ++iter)
	{
		auto position = m_geometry->GetPosition(m_geometry->GetPoint((*iter)));
		if (position == Vec3F{0, 0, 0})
		{
			currentSplitPoint = *iter;
			break;
		}
	}

	std::array<Vec3F, 5> expected = {Vec3F{0, 0, 0}, Vec3F{1, 0, 0}, Vec3F{1, 1, 0}, Vec3F{0.5f, 0.5f, 0},
	                                 Vec3F{0, 1, 0}};

	for (auto i = 0u; i < 5; ++i)
	{
		EXPECT_TRUE(m_geometry->GetPosition(m_geometry->GetPoint(currentSplitPoint)) == expected[i]);
		currentSplitPoint = m_geometry->GetNextSplitPoint(currentSplitPoint);
	}
}

TEST_F(GeometryBuilderTest, when_creating_a_face_should_be_able_to_reuse_points)
{
	auto builder = std::make_shared<GeometryBuilderT<GeometryType>>(m_geometry);
	std::array<GeometryType::Index_t, 6> points = {builder->AddPoint({0, 0, 0}), builder->AddPoint({0, 0, 1}),
	                                               builder->AddPoint({0, 1, 1}), builder->AddPoint({0, 1, 0}),
	                                               builder->AddPoint({1, 1, 1}), builder->AddPoint({1, 0, 1})};

	auto faceBuilder1 = builder->StartFace(5);
	faceBuilder1.AddIndex(points[0]);
	faceBuilder1.AddIndex(points[1]);
	faceBuilder1.AddIndex(points[2]);
	faceBuilder1.AddIndex(points[3]);
	auto faceBuilder2 = builder->StartFace(4);
	faceBuilder2.AddIndex(points[2]);
	faceBuilder2.AddIndex(points[4]);
	faceBuilder2.AddIndex(points[4]);
	faceBuilder2.AddIndex(points[1]);

	faceBuilder1.CloseFace();
	faceBuilder2.CloseFace();

	EXPECT_EQ(m_geometry->GetFaceCount(), 2);
	EXPECT_EQ(m_geometry->GetPointCount(), 6);
	EXPECT_EQ(m_geometry->GetSplitPointCount(), 8);
	EXPECT_EQ(m_geometry->GetEdgeCount(), 8);
	EXPECT_TRUE(m_geometry->IsValid());
}
