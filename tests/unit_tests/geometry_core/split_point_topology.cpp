#include <geometry_core/split_point_topology.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>
#include <algorithm>

#include "../test_utils.h"

using namespace selector;

class SplitPointTopologyCreateFacetest : public ::testing::Test
{
protected:
	void SetUp()
	{
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_a_face_should_return_a_face_with_three_points)
{
	SplitPointTopology::CreateFaceResult result = m_topology.CreateFace();
	
	EXPECT_EQ(result.m_face, 0);
	EXPECT_EQ(result.m_points[0], 0);
	EXPECT_EQ(result.m_points[1], 1);
	EXPECT_EQ(result.m_points[2], 2);
}

TEST_F(SplitPointTopologCreateFaceTest, when_creating_two_faces_should_return_different_indices)
{
	auto results[] = {m_topology.CreateFace(), m_topology.CreateFace()};
	
	for (auto i=0u; i<2; ++i)
	{
		EXPECT_EQ(results[i].m_face, i);
		EXPECT_EQ(results[i].m_points[0], i * 3 + 0);
		EXPECT_EQ(results[i].m_points[1], i * 3 + 1);
		EXPECT_EQ(results[i].m_points[2], i * 3 + 2);
	}
}

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_faces_should_be_able_to_reuse_points)
{
	auto results1 = m_topology.CreateFace();
	auto results2 = m_topology.CreateFace(results1.m_points[0], results.m_points[1]);
	
	EXPECT_EQ(results2.m_points[0], results1.m_points[0]);
	EXPECT_EQ(results2.m_points[1], results1.m_points[1]);
	EXPECT_EQ(results2.m_points[2], 3);
}

class SplitPointTopologyAddPointToFaceTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		face = m_topology.CreateFace();
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_topology;
	SplitPointTopology::CreateFaceResult face;
};

TEST_F(SplitPointTopologyAddPointToFaceTest, when_adding_a_point_to_a_face_should_add_a_new_point)
{
	m_topology.AddPointToFace()
}

class SplitPointTopologyTriangleTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_triangle.CreateFace();
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_triangle;
};