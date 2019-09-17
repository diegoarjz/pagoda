#include <geometry_core/split_point_topology.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>
#include <algorithm>

#include "../test_utils.h"

using namespace selector;

class SplitPointTopologyCreateFaceTest : public ::testing::Test
{
protected:
	void SetUp() {}

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_a_face_should_return_a_face_with_three_points)
{
	SplitPointTopology::CreateFaceResult result = m_topology.CreateFace();

	EXPECT_EQ(result.m_face.GetIndex(), 0);
	EXPECT_EQ(result.m_points[0].GetIndex(), 0);
	EXPECT_EQ(result.m_points[1].GetIndex(), 1);
	EXPECT_EQ(result.m_points[2].GetIndex(), 2);
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_two_faces_should_return_different_indices)
{
	SplitPointTopology::CreateFaceResult results[] = {m_topology.CreateFace(), m_topology.CreateFace()};

	for (auto i = 0u; i < 2; ++i)
	{
		EXPECT_EQ(results[i].m_face.GetIndex(), i);
		EXPECT_EQ(results[i].m_points[0].GetIndex(), i * 3 + 0);
		EXPECT_EQ(results[i].m_points[1].GetIndex(), i * 3 + 1);
		EXPECT_EQ(results[i].m_points[2].GetIndex(), i * 3 + 2);
	}
	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_faces_should_be_able_to_reuse_points)
{
	auto results1 = m_topology.CreateFace();
	auto results2 = m_topology.CreateFace(results1.m_points[0], results1.m_points[1]);

	EXPECT_EQ(results2.m_points[0].GetIndex(), results1.m_points[0].GetIndex());
	EXPECT_EQ(results2.m_points[1].GetIndex(), results1.m_points[1].GetIndex());
	EXPECT_EQ(results2.m_points[2].GetIndex(), 3);
	EXPECT_TRUE(m_topology.IsValid());
}

class SplitPointTopologyAddPointToFaceTest : public ::testing::Test
{
protected:
	void SetUp() {}

	void TearDown() {}

	SplitPointTopology m_topology;
};

/*
TEST_F(SplitPointTopologyAddPointToFaceTest, when_adding_with_an_existing_point_should_return_that_point)
{
	auto newFace = m_topology.CreateFace();
	auto p = m_topology.AddPointToFace(newFace.m_points[0]);
	EXPECT_EQ(p.GetIndex(), m_points[0].GetIndex());

	EXPECT_TRUE(m_topology.IsValid());
}
*/

class SplitPointTopologyOperationsTest : public ::testing::Test
{
protected:
	void SetUp() {}

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologyOperationsTest,
       when_getting_the_split_point_from_a_point_should_get_the_respective_split_point)
{
    /*
    auto face = m_topology.CreateFace();
	for (auto i = 0u; i < 3; ++i)
	{
		SplitPointTopology::SplitPointHandle splitPoint = m_topology.GetSplitPoint(face.m_points[i], face.m_face);
		EXPECT_EQ(m_topology.GetPoint(splitPoint), face.m_points[i]);
	}

	EXPECT_TRUE(m_topology.IsValid());
    */
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_split_point_from_a_face_should_get_the_respective_face)
{
    auto face = m_topology.CreateFace();
	auto splitPoint = m_topology.GetSplitPoint(face.m_face);
	EXPECT_EQ(m_topology.GetFace(splitPoint), face.m_face);

	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_edges_from_a_split_point_should_return_the_respective_edge)
{
    /*
    auto face = m_topology.CreateFace();
	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(face.m_points[i]);
		auto outgoingEdge = m_topology.GetOutgoingEdge(splitPoint);
		auto incomingEdge = m_topology.GetIncomingEdge(splitPoint);

		EXPECT_EQ(m_topology.GetSource(outgoingEdge), splitPoint);
		EXPECT_EQ(m_topology.GetDestination(incomingEdge), splitPoint);
	}

	EXPECT_TRUE(m_topology.IsValid());
    */
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_face_from_an_edge_should_return_the_respective_face)
{
    /*
    auto face = m_topology.CreateFace();
	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(face.m_points[i]);
		auto outgoingEdge = m_topology.GetOutgoingEdge(splitPoint);
		auto incomingEdge = m_topology.GetIncomingEdge(splitPoint);

		EXPECT_EQ(m_topology.GetFace(outgoingEdge), m_face1.m_face)
	}

	EXPECT_TRUE(m_topology.IsValid());
    */
}

class SplitPointTopologyNavigationTest : public ::testing::Test
{
protected:
	void SetUp() {  }

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologyNavigationTest, when_navigating_the_split_points_should_be_able_to_circle_the_face)
{
    //auto &topology = m_topology;
    //auto face = m_topology.CreateFace();
	//auto next = [&topology](SplitPointTopology::SplitPointHandle &p) { return topology.GetNextSplitPoint(p); };
	//auto prev = [&topology](SplitPointTopology::SplitPointHandle &p) { return topology.GetPrevSplitPoint(p); };

    /*
	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(face.m_points[i]);
		EXPECT_EQ(next(next(next(splitPoint))), splitPoint);
		EXPECT_EQ(prev(prev(prev(splitPoint))), splitPoint);
	}
    */

	//EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologyNavigationTest, when_navigating_the_edges_should_be_able_to_circle_the_face)
{
    /*
    auto &topology;
    auto face = m_topology.CreateFace();
	auto next = [&topology](SplitPointTopology::EdgeHandle &e) { return topology.GetNextEdge(e); };
	auto prev = [&topology](SplitPointTopology::EdgeHandle &e) { return topology.GetPrevEdge(e); };

	for (auto i = 0u; i < 3; ++i)
	{
		auto splitPoint = m_topology.GetSplitPoint(face.m_points[i]);
		auto edge = m_topology.GetOutgoingEdge(splitPoint);

		EXPECT_EQ(next(next(next(edge))), edge);
		EXPECT_EQ(prev(prev(prev(edge))), edge);
	}

	EXPECT_TRUE(m_topology.IsValid());
    */
}

class SplitPointTopologySplitEdgeTest : public ::testing::Test
{
protected:
	void SetUp() {  }

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologySplitEdgeTest, when_splitting_an_edge_should_create_a_new_split_point_in_the_same_face)
{
    auto face = m_topology.CreateFace();
	auto edge = m_topology.GetEdge(face.m_face);
	SplitPointTopology::SplitPointHandle s = m_topology.SplitEdge(edge);
	EXPECT_EQ(m_topology.GetOutEdge(s), edge);
	EXPECT_EQ(m_topology.GetFace(s), face.m_face);

	EXPECT_TRUE(m_topology.IsValid());
}

TEST_F(SplitPointTopologySplitEdgeTest, when_splitting_an_edge_should_keep_a_valid_topology)
{
    /*
    auto &topology = m_topology;
    auto face = m_topology.CreateFace();
	auto edge = m_topology.GetEdge(face.m_face);
	SplitPointTopology::SplitPointHandle splitPoint = m_topology.SplitEdge(edge);

	auto next = [&topology](SplitPointTopology::SplitPointHandle &p) { return topology.GetNextSplitPoint(p); };
	auto prev = [&topology](SplitPointTopology::SplitPointHandle &p) { return topology.GetPrevSplitPoint(p); };

	EXPECT_EQ(next(next(next(next(splitPoint)))), splitPoint);
	EXPECT_EQ(prev(prev(prev(prev(splitPoint)))), splitPoint);
    */
}

class SplitPointTopologyCollapseEdgeTest : public ::testing::Test
{
protected:
	void SetUp() { m_triangle.CreateFace(); }

	void TearDown() {}

	SplitPointTopology m_triangle;
};

TEST_F(SplitPointTopologyCollapseEdgeTest, when_collapsing_an_edge_should_remove_the_destination_split_point)
{
    /*
	auto edgeToCollapse = m_triangle.GetEdge(m_triangle.m_face);
	auto nextEdge = m_triangle.GetNextEdge(edgeToCollapse);
	auto splitPointCollapsedTo = m_triangle.CollapseEdge(edgeToCollapse);
	EXPECT_EQ(splitPointCollapsedTo, m_triangle.GetSource(nextEdge);
	EXPECT_TRUE(m_triangle.IsValid());
    */
}

TEST_F(SplitPointTopologyCollapseEdgeTest, when_face_is_a_triangle_should_not_allow_collapsing) {}

class SplitPointTopologySplitFaceTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		//m_topology.AddPointToFace();
	}

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologySplitFaceTest, when_splitting_a_face_should_create_two_edges_between_the_points) {}
