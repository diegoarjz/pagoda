#include <geometry_core/split_point_topology.h>
#include <common/logger.h>
#include <math_lib/vec_base.h>

#include <gtest/gtest.h>
#include <algorithm>

#include "../test_utils.h"

using namespace selector;

TEST(HandleImplicitCastTest, test_implicit_casts)
{
	SplitPointTopology::PointHandle p = 1;
	SplitPointTopology::FaceHandle f = 2;
	SplitPointTopology::EdgeHandle e = 3;
	SplitPointTopology::SplitPointHandle s = 4;

	EXPECT_EQ(p.GetIndex(), 1);
	EXPECT_EQ(f.GetIndex(), 2);
	EXPECT_EQ(e.GetIndex(), 3);
	EXPECT_EQ(s.GetIndex(), 4);

	SplitPointTopology::Index_t i = p;
	EXPECT_EQ(i, 1);
	i = f;
	EXPECT_EQ(i, 2);
	i = e;
	EXPECT_EQ(i, 3);
	i = s;
	EXPECT_EQ(i, 4);
}

class SplitPointTopologyCreateFaceTest : public SelectorTestFixture<::testing::Test>
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
	EXPECT_EQ(result.m_splitPoints[0].GetIndex(), 0);
	EXPECT_EQ(result.m_splitPoints[1].GetIndex(), 1);
	EXPECT_EQ(result.m_splitPoints[2].GetIndex(), 2);
	EXPECT_TRUE(m_topology.IsValid());

    std::stringstream ss;
    m_topology.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_two_faces_should_return_different_indices)
{
	SplitPointTopology::CreateFaceResult results[] = {m_topology.CreateFace(), m_topology.CreateFace()};

	for (auto i = 0u; i < 2; ++i)
	{
		EXPECT_EQ(results[i].m_face.GetIndex(), i);
		EXPECT_EQ(results[i].m_splitPoints[0].GetIndex(), i * 3 + 0);
		EXPECT_EQ(results[i].m_splitPoints[1].GetIndex(), i * 3 + 1);
		EXPECT_EQ(results[i].m_splitPoints[2].GetIndex(), i * 3 + 2);
	}
	EXPECT_TRUE(m_topology.IsValid());

    std::stringstream ss;
    m_topology.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyCreateFaceTest, when_creating_faces_should_be_able_to_reuse_points)
{
	auto results1 = m_topology.CreateFace();
	auto results2 = m_topology.CreateFace(
            m_topology.GetPoint(results1.m_splitPoints[0]),
            m_topology.GetPoint(results1.m_splitPoints[1]));

	EXPECT_EQ(m_topology.GetPoint(results2.m_splitPoints[0].GetIndex()), m_topology.GetPoint(results1.m_splitPoints[0].GetIndex()));
	EXPECT_EQ(m_topology.GetPoint(results2.m_splitPoints[1].GetIndex()), m_topology.GetPoint(results1.m_splitPoints[1].GetIndex()));
	EXPECT_EQ(m_topology.GetPoint(results2.m_splitPoints[2].GetIndex()).GetIndex(), 3);
	EXPECT_TRUE(m_topology.IsValid());

    std::stringstream ss;
    m_topology.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

class SplitPointTopologyOperationsTest : public SelectorTestFixture<::testing::Test>
{
protected:
	void SetUp() {}

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_split_point_from_a_point_should_get_the_respective_split_point)
{
	auto face = m_topology.CreateFace();

	for (auto i = 0u; i < 3; ++i)
	{
        auto point = m_topology.GetPoint(face.m_splitPoints[i]);
        auto edge = *(m_topology.GetOutEdges(point).begin());
        EXPECT_EQ(point, m_topology.GetPoint(m_topology.GetSource(edge)));
	}
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_split_point_from_a_face_should_get_the_respective_face)
{
	auto face = m_topology.CreateFace();
	auto splitPoint = m_topology.GetSplitPoint(face.m_face);
	EXPECT_EQ(m_topology.GetFace(splitPoint), face.m_face);
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_edges_from_a_split_point_should_return_the_respective_edge)
{
	auto face = m_topology.CreateFace();
	for (auto i = 0u; i < 3; ++i)
	{
        auto point = m_topology.GetPoint(face.m_splitPoints[i]);
        auto edge = *(m_topology.GetOutEdges(point).begin());
	    auto splitPoint = m_topology.GetSource(edge);
	    auto outgoingEdge = m_topology.GetOutEdge(splitPoint);
	    auto incomingEdge = m_topology.GetInEdge(splitPoint);

	    EXPECT_EQ(m_topology.GetSource(outgoingEdge), splitPoint);
	    EXPECT_EQ(m_topology.GetDestination(incomingEdge), splitPoint);
	}
}

TEST_F(SplitPointTopologyOperationsTest, when_getting_the_face_from_an_edge_should_return_the_respective_face)
{
	auto face = m_topology.CreateFace();
	for (auto i = 0u; i < 3; ++i)
	{
        auto point = m_topology.GetPoint(face.m_splitPoints[i]);
        auto edge = *(m_topology.GetOutEdges(point).begin());
	    EXPECT_EQ(m_topology.GetFace(edge), face.m_face);
	}
}

class SplitPointTopologyNavigationTest : public SelectorTestFixture<::testing::Test>
{
protected:
	void SetUp() {}

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologyNavigationTest, when_navigating_the_split_points_should_be_able_to_circle_the_face)
{
	auto &topology = m_topology;
	auto face = m_topology.CreateFace();
	auto next = [&topology](SplitPointTopology::SplitPointHandle p) { return topology.GetNextSplitPoint(p); };
	auto prev = [&topology](SplitPointTopology::SplitPointHandle p) { return topology.GetPrevSplitPoint(p); };

	for (auto i = 0u; i < 3; ++i)
	{
        auto point = m_topology.GetPoint(face.m_splitPoints[i]);
        auto edge = *(m_topology.GetOutEdges(point).begin());
        SplitPointTopology::SplitPointHandle splitPoint = m_topology.GetSource(edge);
	    EXPECT_EQ(next(next(next(splitPoint))), splitPoint);
	    EXPECT_EQ(prev(prev(prev(splitPoint))), splitPoint);
	}
}

TEST_F(SplitPointTopologyNavigationTest, when_navigating_the_edges_should_be_able_to_circle_the_face)
{
	auto &topology = m_topology;
	auto face = m_topology.CreateFace();
	auto next = [&topology](SplitPointTopology::EdgeHandle e) { return topology.GetNextEdge(e); };
	auto prev = [&topology](SplitPointTopology::EdgeHandle e) { return topology.GetPrevEdge(e); };

	for (auto i = 0u; i < 3; ++i)
	{
        auto point = m_topology.GetPoint(face.m_splitPoints[i]);
        auto edge = *(m_topology.GetOutEdges(point).begin());

	    EXPECT_EQ(next(next(next(edge))), edge);
	    EXPECT_EQ(prev(prev(prev(edge))), edge);
	}
}

class SplitPointTopologyIteratorsTest : public SelectorTestFixture<::testing::Test>
{
protected:
    void SetUp()
    {
        m_face = m_topology.CreateFace();
    }

    void TearDown() {}

    SplitPointTopology m_topology;
    SplitPointTopology::CreateFaceResult m_face;
};

TEST_F(SplitPointTopologyIteratorsTest, when_iterating_over_points_should_go_through_all)
{
    auto iter = m_topology.PointsBegin();
    auto endIter = m_topology.PointsEnd();
    std::unordered_set<SplitPointTopology::Index_t> seenPoints;

    for (/**/; iter != endIter; ++iter)
    {
        EXPECT_TRUE(seenPoints.find((*iter).GetIndex()) == seenPoints.end());
        seenPoints.insert((*iter).GetIndex());
    }
    EXPECT_EQ(seenPoints.size(), 3);
}

TEST_F(SplitPointTopologyIteratorsTest, when_iterating_over_split_points_should_go_through_all)
{
    auto iter = m_topology.SplitPointsBegin();
    auto endIter = m_topology.SplitPointsEnd();
    std::unordered_set<SplitPointTopology::Index_t> seenSplitPoints;

    for (/**/; iter != endIter; ++iter)
    {
        EXPECT_TRUE(seenSplitPoints.find((*iter).GetIndex()) == seenSplitPoints.end());
        seenSplitPoints.insert((*iter).GetIndex());
    }
    EXPECT_EQ(seenSplitPoints.size(), 3);
}

TEST_F(SplitPointTopologyIteratorsTest, when_iterating_over_edges_should_go_through_all)
{
    auto iter = m_topology.EdgesBegin();
    auto endIter = m_topology.EdgesEnd();
    std::unordered_set<SplitPointTopology::Index_t> seenEdges;

    for (/**/; iter != endIter; ++iter)
    {
        EXPECT_TRUE(seenEdges.find((*iter).GetIndex()) == seenEdges.end());
        seenEdges.insert((*iter).GetIndex());
    }
    EXPECT_EQ(seenEdges.size(), 3);
}

TEST_F(SplitPointTopologyIteratorsTest, when_iterating_over_faces_should_go_through_all)
{
    m_topology.CreateFace();
    auto iter = m_topology.FacesBegin();
    auto endIter = m_topology.FacesEnd();
    std::unordered_set<SplitPointTopology::Index_t> seenFaces;

    for (/**/; iter != endIter; ++iter)
    {
        EXPECT_TRUE(seenFaces.find((*iter).GetIndex()) == seenFaces.end());
        seenFaces.insert((*iter).GetIndex());
    }
    EXPECT_EQ(seenFaces.size(), 2);
}

TEST_F(SplitPointTopologyIteratorsTest, when_iterating_over_point_edges_should_go_through_all)
{
    m_topology.CreateFace(m_topology.GetPoint(m_face.m_splitPoints[0]));
    auto iter = m_topology.PointEdgesBegin(m_topology.GetPoint(m_face.m_splitPoints[0]));
    auto endIter = m_topology.PointEdgesEnd(m_topology.GetPoint(m_face.m_splitPoints[0]));
    std::unordered_set<SplitPointTopology::Index_t> seenEdges;

    for (/**/; iter != endIter; ++iter)
    {
        EXPECT_TRUE(seenEdges.find((*iter).GetIndex()) == seenEdges.end());
        seenEdges.insert((*iter).GetIndex());
    }
    EXPECT_EQ(seenEdges.size(), 2);
}

TEST_F(SplitPointTopologyIteratorsTest, when_iterating_over_point_split_points_should_go_through_all)
{
    m_topology.CreateFace(m_topology.GetPoint(m_face.m_splitPoints[0]));
    auto iter = m_topology.PointSplitPointBegin(m_topology.GetPoint(m_face.m_splitPoints[0]));
    auto endIter = m_topology.PointSplitPointEnd(m_topology.GetPoint(m_face.m_splitPoints[0]));
    std::unordered_set<SplitPointTopology::Index_t> seenSplitPoints;

    for (/**/; iter != endIter; ++iter)
    {
        EXPECT_TRUE(seenSplitPoints.find((*iter).GetIndex()) == seenSplitPoints.end());
        seenSplitPoints.insert((*iter).GetIndex());
    }
    EXPECT_EQ(seenSplitPoints.size(), 2);
}

TEST_F(SplitPointTopologyIteratorsTest, when_iterating_over_point_faces_should_go_through_all)
{
    m_topology.CreateFace(m_topology.GetPoint(m_face.m_splitPoints[0]));
    auto iter = m_topology.PointFaceBegin(m_topology.GetPoint(m_face.m_splitPoints[0]));
    auto endIter = m_topology.PointFaceEnd(m_topology.GetPoint(m_face.m_splitPoints[0]));
    std::unordered_set<SplitPointTopology::Index_t> seenFaces;

    for (/**/; iter != endIter; ++iter)
    {
        EXPECT_TRUE(seenFaces.find((*iter).GetIndex()) == seenFaces.end());
        seenFaces.insert((*iter).GetIndex());
    }
    EXPECT_EQ(seenFaces.size(), 2);
}

TEST_F(SplitPointTopologyIteratorsTest, when_circulating_over_face_edges_should_go_through_all)
{
    auto iter = m_topology.FaceEdgeCirculatorBegin(m_face.m_face);
    std::unordered_set<SplitPointTopology::Index_t> seenEdges;

    while (iter)
    {
        EXPECT_TRUE(seenEdges.find((*iter).GetIndex()) == seenEdges.end());
        seenEdges.insert((*iter).GetIndex());
        ++iter;
    }
    EXPECT_EQ(seenEdges.size(), 3);
}

TEST_F(SplitPointTopologyIteratorsTest, when_circulating_over_face_split_points_should_go_through_all)
{
    auto iter = m_topology.FaceSplitPointCirculatorBegin(m_face.m_face);
    std::unordered_set<SplitPointTopology::Index_t> seenSplitPoints;

    while (iter)
    {
        EXPECT_TRUE(seenSplitPoints.find((*iter).GetIndex()) == seenSplitPoints.end());
        seenSplitPoints.insert((*iter).GetIndex());
        ++iter;
    }
    EXPECT_EQ(seenSplitPoints.size(), 3);
}

TEST_F(SplitPointTopologyIteratorsTest, when_circulating_over_face_points_should_go_through_all)
{
    auto iter = m_topology.FacePointCirculatorBegin(m_face.m_face);
    std::unordered_set<SplitPointTopology::Index_t> seenPoints;

    while (iter)
    {
        EXPECT_TRUE(seenPoints.find((*iter).GetIndex()) == seenPoints.end());
        seenPoints.insert((*iter).GetIndex());
        ++iter;
    }
    EXPECT_EQ(seenPoints.size(), 3);
}

class SplitPointTopologySplitEdgeTest : public SelectorTestFixture<::testing::Test>
{
protected:
	void SetUp() {}

	void TearDown() {}

	SplitPointTopology m_topology;
};

TEST_F(SplitPointTopologySplitEdgeTest, when_splitting_an_edge_should_create_a_new_split_point_in_the_same_face)
{
	auto face = m_topology.CreateFace();
	auto edge = m_topology.GetEdge(face.m_face);
	SplitPointTopology::SplitPointHandle s = m_topology.SplitEdge(edge);
	EXPECT_EQ(m_topology.GetFace(s), face.m_face);
	EXPECT_TRUE(m_topology.IsValid());

    std::stringstream ss;
    m_topology.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

class SplitPointTopologyCollapseEdgeTest : public SelectorTestFixture<::testing::Test>
{
protected:
	void SetUp()
	{
		m_face = m_triangle.CreateFace();
		m_triangle.SplitEdge(0);
	}

	void TearDown() {}

	SplitPointTopology m_triangle;
	SplitPointTopology::CreateFaceResult m_face;
};

TEST_F(SplitPointTopologyCollapseEdgeTest, when_collapsing_an_edge_should_remove_the_destination_split_point)
{
	auto edgeToCollapse = m_triangle.GetEdge(m_face.m_face);
	m_triangle.CollapseEdge(edgeToCollapse);
	EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyCollapseEdgeTest, when_face_is_a_triangle_should_not_allow_collapsing) {}

class SplitPointTopologyDeleteTest : public SelectorTestFixture<::testing::Test>
{
protected:
	void SetUp()
	{
		m_face = m_triangle.CreateFace();
	}

	void TearDown() {}

	SplitPointTopology m_triangle;
	SplitPointTopology::CreateFaceResult m_face;
};

TEST_F(SplitPointTopologyDeleteTest, when_deleting_a_face_should_cascade)
{
    m_triangle.DeleteFace(m_face.m_face);
    EXPECT_EQ(m_triangle.GetFaceCount(), 0);
    EXPECT_EQ(m_triangle.GetPointCount(), 0);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 0);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 0);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyDeleteTest, when_deleting_a_face_should_not_affect_adjacent_faces)
{
    m_triangle.CreateFace(m_triangle.GetPoint(m_face.m_splitPoints[0]), m_triangle.GetPoint(m_face.m_splitPoints[1]));
    m_triangle.DeleteFace(m_face.m_face);
    EXPECT_EQ(m_triangle.GetFaceCount(), 1);
    EXPECT_EQ(m_triangle.GetPointCount(), 3);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 3);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 3);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyDeleteTest, when_deleting_a_point_should_cascade)
{
    m_triangle.DeletePoint(m_triangle.GetPoint(m_face.m_splitPoints[0]));
    EXPECT_EQ(m_triangle.GetFaceCount(), 0);
    EXPECT_EQ(m_triangle.GetPointCount(), 0);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 0);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 0);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyDeleteTest, when_deleting_a_point_should_not_affect_unrelated_faces)
{
    m_triangle.CreateFace(m_triangle.GetPoint(m_face.m_splitPoints[0]), m_triangle.GetPoint(m_face.m_splitPoints[1]));
    m_triangle.DeletePoint(m_triangle.GetPoint(m_face.m_splitPoints[2]));
    EXPECT_EQ(m_triangle.GetFaceCount(), 1);
    EXPECT_EQ(m_triangle.GetPointCount(), 3);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 3);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 3);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyDeleteTest, when_deleting_a_split_point_should_cascade)
{
    m_triangle.DeleteSplitPoint(m_triangle.GetSplitPoint(m_face.m_face));
    EXPECT_EQ(m_triangle.GetFaceCount(), 0);
    EXPECT_EQ(m_triangle.GetPointCount(), 0);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 0);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 0);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyDeleteTest, when_deleting_a_split_point_should_not_affect_unrelated_faces)
{
    m_triangle.CreateFace(m_triangle.GetPoint(m_face.m_splitPoints[1]), m_triangle.GetPoint(m_face.m_splitPoints[2]));
    m_triangle.DeleteSplitPoint(m_triangle.GetSplitPoint(m_face.m_face));
    EXPECT_EQ(m_triangle.GetFaceCount(), 1);
    EXPECT_EQ(m_triangle.GetPointCount(), 3);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 3);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 3);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyDeleteTest, when_deleting_an_edge_should_cascade)
{
    m_triangle.DeleteEdge(m_triangle.GetEdge(m_face.m_face));
    EXPECT_EQ(m_triangle.GetFaceCount(), 0);
    EXPECT_EQ(m_triangle.GetPointCount(), 0);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 0);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 0);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

TEST_F(SplitPointTopologyDeleteTest, when_deleting_an_edge_should_not_affect_unrelated_faces)
{
    m_triangle.CreateFace(m_triangle.GetPoint(m_face.m_splitPoints[1]), m_triangle.GetPoint(m_face.m_splitPoints[2]));
    m_triangle.DeleteEdge(m_triangle.GetEdge(m_face.m_face));
    EXPECT_EQ(m_triangle.GetFaceCount(), 1);
    EXPECT_EQ(m_triangle.GetPointCount(), 3);
    EXPECT_EQ(m_triangle.GetSplitPointCount(), 3);
    EXPECT_EQ(m_triangle.GetEdgeCount(), 3);
    EXPECT_TRUE(m_triangle.IsValid());

    std::stringstream ss;
    m_triangle.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}

class SplitPointTopologySplitFaceTest : public SelectorTestFixture<::testing::Test>
{
protected:
	void SetUp()
	{
        m_face = m_topology.CreateFace();
        m_topology.SplitEdge(m_topology.GetEdge(m_face.m_face));
	}

	void TearDown() {}

	SplitPointTopology m_topology;
    SplitPointTopology::CreateFaceResult m_face;
};

TEST_F(SplitPointTopologySplitFaceTest, when_splitting_a_face_should_create_two_edges_between_the_points)
{
    auto e0 = m_topology.GetEdge(m_face.m_face);
    auto e1 = m_topology.GetNextEdge(e0);

    m_topology.SplitFace(m_face.m_face, e0, e1);

    ASSERT_TRUE(m_topology.IsValid());
    EXPECT_EQ(m_topology.GetFaceCount(), 2);
    EXPECT_EQ(m_topology.GetPointCount(), 4);
    EXPECT_EQ(m_topology.GetSplitPointCount(), 6);
    EXPECT_EQ(m_topology.GetEdgeCount(), 6);

    std::stringstream ss;
    m_topology.DumpToStream(ss);
    MatchFile match(GetCurrentTestFileResultsDirectory() /= "topology.txt", GetShouldWriteFiles());
    match.Match(ss.str());
}
