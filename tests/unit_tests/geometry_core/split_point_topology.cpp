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
	
	EXPECT_TRUE(m_topology.IsValid());
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
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_faces_should_be_able_to_reuse_points)
{
	auto results1 = m_topology.CreateFace();
	auto results2 = m_topology.CreateFace(results1.m_points[0], results.m_points[1]);
	
	EXPECT_EQ(results2.m_points[0], results1.m_points[0]);
	EXPECT_EQ(results2.m_points[1], results1.m_points[1]);
	EXPECT_EQ(results2.m_points[2], 3);
	
	EXPECT_TRUE(m_topology.IsValid());
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

TEST_F(SplitPointTopologyAddPointToFaceTest, when_adding_a_point_should_add_a_new_point)
{
	SplitPointTopology::SplitPointHandle p = m_topology.AddPointToFace()
	EXPECT_EQ(p.GetIndex(), 4);
	
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyAddPointToFaceTest, when_adding_with_an_existing_point_should_return_that_point)
{
	auto newFace = m_topology.CreateFace();
	auto p = m_topology.AddPointToFace(newFace.m_points[0]);
	EXPECT_EQ(p.GetIndex(), m_points[0].GetIndex());	
	
	EXPECT_TRUE(m_topology.IsValid());
}

class SplitPointTopologyOperationsTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_face1.CreateFace();
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_topology;
	SplitPointTopology::CreateFaceResult m_face1;
};

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_split_point_from_a_point_should_get_the_respective_split_point)
{
	for (auto i = 0u; i < 3; ++i)
	{
		SplitPointTopology::SplitPointHandle splitPoint = m_topology.GetSplitPoint(m_face1.m_points[i]);
		EXPECT_EQ(m_topology.GetPoint(splitPoint), m_face1.m_points[i]);
	}
	
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_split_point_from_a_face_should_get_the_respective_face)
{
	auto splitPoint = m_topology.GetSplitPoint(m_face1.m_face);
	EXPECT_EQ(m_topology.GetFace(splitPoint), m_face1.m_face);
	
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_edges_from_a_split_point_should_return_the_respective_edge)
{
	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(m_face1.m_points[i]);
		auto outgoingEdge = m_topology.GetOutgoingEdge(splitPoint);
		auto incomingEdge = m_topology.GetIncomingEdge(splitPoint);
		
		EXPECT_EQ(m_topology.GetSource(outgoingEdge), splitPoint);
		EXPECT_EQ(m_topology.GetDestination(incomingEdge), splitPoint);
	}
	
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyOperationstest, when_getting_the_face_from_an_edge_should_return_the_respective_face)
{
	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(m_face1.m_points[i]);
		auto outgoingEdge = m_topology.GetOutgoingEdge(splitPoint);
		auto incomingEdge = m_topology.GetIncomingEdge(splitPoint);
		
		EXPECT_EQ(m_topology.GetFace(outgoingEdge), m_face1.m_face)
	}
	
	EXPECT_TRUE(m_topology.IsValid());
}

class SplitPointTopologyNavigationTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_face1 = m_topology.CreateFace();
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_topology;
	SplitPointTopology::CreateFaceResult m_face1;
};

TEST_F(SplitPointTopologyNavigationTest, when_navigating_the_split_points_should_be_able_to_circle_the_face)
{
	auto next = [&m_topology](SplitPointTopology::SplitPointHandle &p) {
		return m_topology.GetNextSplitPoint(p);
	};
	auto prev = [&m_topology](SplitPointTopology::SplitPointHandle &p) {
		return m_topology.GetPrevSplitPoint(p);
	};
	
	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(m_face1.m_points[i]);
		EXPECT_EQ(next(next(next(splitPoint))), splitPoint);
		EXPECT_EQ(prev(prev(prev(splitPoint))), splitPoint);
	}
	
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyNavigationTest, when_navigating_the_edges_should_be_able_to_circle_the_face)
{
	auto next = [&m_topology](SplitPointTopology::EdgeHandle &e) {
		return m_topology.GetNextEdge(e);
	};
	auto prev = [&m_topology](SplitPointTopology::EdgeHandle &e) {
		return m_topology.GetPrevEdge(e);
	};
	
	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(m_face1.m_points[i]);
		auto edge = m_topology.GetOutgoingEdge(splitPoint);
		
		EXPECT_EQ(next(next(next(edge))), edge);
		EXPECT_EQ(prev(prev(prev(edge))), edge);
	}
	
	EXPECT_TRUE(m_topology.IsValid());
}

class SplitPointTopologySplitEdgeTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_face1 = m_topology.CreateFace();
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_topology;
	SplitPointTopology::CreateFaceResult m_face1;
};

TEST_F(SplitPointTopologySplitEdgeTest, when_splitting_an_edge_should_create_a_new_split_point_in_the_same_face)
{
	auto edge = m_topology.GetEdge(m_face1.m_face);
	SplitPointTopology::SplitPointHandle s = m_topology.SplitEdge(edge);
	EXPECT_EQ(m_topology.GetOutgoingEdge(s), edge);
	EXPECT_EQ(m_topology.GetFace(s), m_face1.m_face);
	
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologySplitEdgeTest, when_splitting_an_edge_should_keep_a_valid_topology)
{
	auto edge = m_topology.GetEdge(m_face1.m_face);
	SplitPointTopology::SplitPointHandle splitPoint = m_topology.SplitEdge(edge);

	auto next = [&m_topology](SplitPointTopology::SplitPointHandle &p) {
		return m_topology.GetNextSplitPoint(p);
	};
	auto prev = [&m_topology](SplitPointTopology::SplitPointHandle &p) {
		return m_topology.GetPrevSplitPoint(p);
	};
	
	EXPECT_EQ(next(next(next(next(splitPoint)))), splitPoint);
	EXPECT_EQ(prev(prev(prev(prev(splitPoint)))), splitPoint);
}

class SplitPointTopologyCollapseEdgeTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_face1 = m_topology.CreateFace();
		m_topology.AddPointToFace(m_face1.m_face, m_topology.GetEdge(m_face1.m_face));
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_topology;
	SplitPointTopology m_face1;
};


TEST_F(SplitPointTopologyCollapseEdgeTest, when_collapsing_an_edge_should_remove_the_destination_split_point)
{
	auto edgeToCollapse = m_topology.GetEdge(m_topology.m_face);
	auto nextEdge = m_topology.GetNextEdge(edgeToCollapse);
	auto splitPointCollapsedTo = m_topology.CollapseEdge(edgeToCollapse);
	EXPECT_EQ(splitPointCollapsedTo, m_topology.GetSource(nextEdge);
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyCollapsedEdgeTest, when_face_is_a_triangle_should_not_allow_collapsing)
{
}

class SplitPointTopologySplitFaceTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_face1 = m_topology.CreateFace();
		m_topology.AddPointToFace();
	}
	
	void TearDown()
	{
	}
	
	SplitPointTopology m_topology;
	SplitPointTopology::CreateFaceResult m_face1;
};

TEST_F(SplitPointTopologySplitFaceTest, when_splitting_a_face_should_create_two_edges_between_the_points)
{
}
