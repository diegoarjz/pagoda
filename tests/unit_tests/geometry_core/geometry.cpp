#include <geometry_core/geometry.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>
#include <algorithm>

#include "../test_utils.h"

using namespace selector;

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

TEST_F(GeometryTest, when_setting_position_should_store_them)
{
    /*
    m_geometry->SetPosition(m_face.m_points[0], Vec3F(1,2,3));
    EXPECT_EQ(m_geometry->GetPosition(m_face.m_points[0]), Vec3F(1,2,3));
    */
}

TEST_F(GeometryTest, when_getting_attributes_should_create_them)
{
    /*
    auto &faceAttr = m_geometry->GetFaceAttributes(m_face.m_face);
    faceAttr.m_normal = Vec3F(1,0,0);
    EXPECT_EQ(m_geometry->GetFaceAttributes(m_face.m_face).m_normal, Vec3F(1,0,0));
    */
}

