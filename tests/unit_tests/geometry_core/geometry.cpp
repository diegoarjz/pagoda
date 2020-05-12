#include <pagoda/geometry_core/geometry.h>
#include <pagoda/math_lib/vec_base.h>

#include <gtest/gtest.h>
#include <algorithm>

#include "../test_utils.h"

using namespace pagoda;

class GeometryTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		m_geometry = std::make_shared<GeometryBase<>>();
		m_face = m_geometry->CreateFace();
	}

	virtual void TearDown() {}

	std::shared_ptr<GeometryBase<>> m_geometry;
	SplitPointTopology::CreateFaceResult m_face;
};

TEST_F(GeometryTest, when_created_should_be_empty)
{
	auto emptyGeometry = std::make_shared<GeometryBase<>>();
	EXPECT_EQ(emptyGeometry->GetFaceCount(), 0);
	EXPECT_EQ(emptyGeometry->GetEdgeCount(), 0);
	EXPECT_EQ(emptyGeometry->GetSplitPointCount(), 0);
	EXPECT_EQ(emptyGeometry->GetPointCount(), 0);
}

