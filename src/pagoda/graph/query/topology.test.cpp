#include <pagoda/pagoda.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/query/query.h>
#include <pagoda/graph/query/topology.h>
#include <pagoda/graph/query/type.h>

#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_node.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class QueryTopologyTest : public ::testing::Test
{
	protected:
	void SetUp() override
	{
		//
	}
	void TearDown() override
	{
		//
	}

	Query m_query1;
	Query m_query2;
	Query m_query3;
};

TEST_F(QueryTopologyTest, stores_the_initial_query)
{
	QueryTopology t(&m_query1);
	EXPECT_EQ(t.GetQueries().size(), 1);
	EXPECT_EQ(*(t.GetQueries().begin()), &m_query1);
}

TEST_F(QueryTopologyTest, can_create_adjacencies)
{
	QueryTopology t(&m_query1);
	t.AddDownstreamQuery(&m_query1, &m_query2);

	std::unordered_set<Query *> downStream;
	std::unordered_set<Query *> upstream;

	t.GetDownstreamQueriesOf(&m_query1, std::inserter(downStream, downStream.end()));
	t.GetUpstreamQueriesOf(&m_query2, std::inserter(upstream, upstream.end()));

	ASSERT_EQ(downStream.size(), 1);
	ASSERT_EQ(upstream.size(), 1);
	EXPECT_EQ(*(downStream.begin()), &m_query2);
	EXPECT_EQ(*(upstream.begin()), &m_query1);
	EXPECT_EQ(t.GetQueries().size(), 2);

	t.AddUpstreamQuery(&m_query1, &m_query3);
	downStream.clear();
	upstream.clear();

	t.GetDownstreamQueriesOf(&m_query3, std::inserter(downStream, downStream.end()));
	t.GetUpstreamQueriesOf(&m_query1, std::inserter(upstream, upstream.end()));

	ASSERT_EQ(downStream.size(), 1);
	ASSERT_EQ(upstream.size(), 1);
	EXPECT_EQ(*(downStream.begin()), &m_query1);
	EXPECT_EQ(*(upstream.begin()), &m_query3);
}

class QueryTopologyMatchTest : public ::testing::Test
{
	protected:
	void SetUp() override
	{
		m_graph = m_pagoda.CreateGraph();
		m_graph->CreateNode<ParameterNode>("par");
		m_graph->CreateNode<OperationNode>("op");
		m_graph->CreateNode<InputInterfaceNode>("in");
		m_graph->CreateNode<OutputInterfaceNode>("out");

		m_graph->CreateEdge("par", "op");
		m_graph->CreateEdge("in", "op");
		m_graph->CreateEdge("op", "out");
	}
	void TearDown() override
	{
		//
	}

	Pagoda m_pagoda;
	GraphPtr m_graph;
};

TEST_F(QueryTopologyMatchTest, should_match_any)
{
	Query query;
	QueryTopology t(&query);
	std::vector<typename QueryTopology::Match_t> matches;
	t.Match(m_graph.get(), matches);

	ASSERT_EQ(matches.size(), 4);
	for (auto &m : matches) {
		EXPECT_NE(m.find(&query), m.end());
		EXPECT_NE(m[&query], nullptr);
	}
	// TODO: add proper test of matching between query and node
}

TEST_F(QueryTopologyMatchTest, should_match_one)
{
	Type<OperationNode> query;
	QueryTopology t(&query);

	std::vector<typename QueryTopology::Match_t> matches;
	t.Match(m_graph.get(), matches);
	ASSERT_EQ(matches.size(), 1);
	EXPECT_EQ(matches[0][&query], m_graph->GetNode("op"));
}

TEST_F(QueryTopologyMatchTest, should_match_with_a_single_downstream_query)
{
	Type<OperationNode> opQuery;
	Type<OutputInterfaceNode> outQuery;
	QueryTopology t(&opQuery);
	t.AddDownstreamQuery(&opQuery, &outQuery);

	std::vector<typename QueryTopology::Match_t> matches;
	t.Match(m_graph.get(), matches);

	ASSERT_EQ(matches.size(), 1);
	EXPECT_EQ(matches[0][&opQuery], m_graph->GetNode("op"));
	EXPECT_EQ(matches[0][&outQuery], m_graph->GetNode("out"));
}

TEST_F(QueryTopologyMatchTest, should_match_with_chained_downstream_queries)
{
	Type<InputInterfaceNode> inQuery;
	Type<OperationNode> opQuery;
	Type<OutputInterfaceNode> outQuery;
	QueryTopology t(&opQuery);
	t.AddDownstreamQuery(&opQuery, &outQuery);
	t.AddUpstreamQuery(&opQuery, &inQuery);

	std::vector<typename QueryTopology::Match_t> matches;
	t.Match(m_graph.get(), matches);
	ASSERT_EQ(matches.size(), 1);
	EXPECT_EQ(matches[0][&opQuery], m_graph->GetNode("op"));
	EXPECT_EQ(matches[0][&outQuery], m_graph->GetNode("out"));
	EXPECT_EQ(matches[0][&inQuery], m_graph->GetNode("in"));
}

TEST_F(QueryTopologyMatchTest, should_match_with_alternatives)
{
	m_graph->CreateNode<OutputInterfaceNode>("out2");
	m_graph->CreateEdge("op", "out2");

	Type<OperationNode> opQuery;
	Type<OutputInterfaceNode> outQuery;
	QueryTopology t(&opQuery);
	t.AddDownstreamQuery(&opQuery, &outQuery);

	std::vector<typename QueryTopology::Match_t> matches;
	t.Match(m_graph.get(), matches);

	ASSERT_EQ(matches.size(), 2);
	for (auto m : matches) {
		EXPECT_EQ(m[&opQuery], m_graph->GetNode("op"));
		EXPECT_TRUE(m[&outQuery] == m_graph->GetNode("out") || m[&outQuery] == m_graph->GetNode("out2"));
	}
	EXPECT_NE(matches[0][&outQuery], matches[1][&outQuery]);
}

TEST_F(QueryTopologyMatchTest, should_match_with_a_downstream_and_an_upstream_query)
{
	Type<OperationNode> opQuery;
	Type<OutputInterfaceNode> outQuery;
	Type<InputInterfaceNode> inQuery;
	QueryTopology t(&opQuery);
	t.AddDownstreamQuery(&opQuery, &outQuery);
	t.AddUpstreamQuery(&opQuery, &inQuery);

	std::vector<typename QueryTopology::Match_t> matches;
	t.Match(m_graph.get(), matches);

	ASSERT_EQ(matches.size(), 1);
	EXPECT_EQ(matches[0][&opQuery], m_graph->GetNode("op"));
	EXPECT_EQ(matches[0][&outQuery], m_graph->GetNode("out"));
	EXPECT_EQ(matches[0][&inQuery], m_graph->GetNode("in"));
}

TEST_F(QueryTopologyMatchTest, should_match_with_multiple_paths)
{
	m_graph->CreateNode<OutputInterfaceNode>("out2");
	m_graph->CreateEdge("op", "out2");
	m_graph->CreateNode<InputInterfaceNode>("in2");
	m_graph->CreateEdge("out", "in2");
	m_graph->CreateEdge("out2", "in2");

	Type<OperationNode> opQuery;
	Type<OutputInterfaceNode> outQuery;
	Type<InputInterfaceNode> inQuery;
	QueryTopology t(&opQuery);
	t.AddDownstreamQuery(&opQuery, &outQuery);
	t.AddDownstreamQuery(&outQuery, &inQuery);

	std::vector<typename QueryTopology::Match_t> matches;
	t.Match(m_graph.get(), matches);
	ASSERT_EQ(matches.size(), 2);
	for (auto &m : matches) {
		EXPECT_EQ(m[&opQuery], m_graph->GetNode("op"));
		EXPECT_EQ(m[&inQuery], m_graph->GetNode("in2"));
		EXPECT_TRUE(m[&outQuery] == m_graph->GetNode("out") || m[&outQuery] == m_graph->GetNode("out2"));
	}
	EXPECT_NE(matches[0][&outQuery], matches[1][&outQuery]);
}
