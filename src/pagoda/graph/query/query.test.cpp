#include "query.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>
#include <unordered_set>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class QueryTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = m_pagoda.CreateGraph();

		m_nodes.insert(m_graph->GetNode(m_graph->CreateNode<OperationNode>("n1")));
		m_nodes.insert(m_graph->GetNode(m_graph->CreateNode<OperationNode>("n1")));
		m_nodes.insert(m_graph->GetNode(m_graph->CreateNode<OperationNode>("n1")));
	}

	void TearDown() {}

	GraphPtr m_graph;
	std::unordered_set<NodePtr> m_nodes;
	Pagoda m_pagoda;
};

TEST_F(QueryTest, should_match_all_nodes)
{
	NodeSet nodes;
	Query q(*m_graph, nodes);
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(nodes.size(), m_nodes.size());
	for (const auto &n : m_nodes) {
		EXPECT_NE(nodes.find(n), nodes.end());
	}

	std::unordered_set<NodePtr> visited;
	Query q2(*m_graph, [&](NodePtr n) { visited.insert(n); });
	m_graph->ExecuteQuery(q2);
	EXPECT_EQ(m_nodes.size(), visited.size());
	for (const auto &n : m_nodes) {
		EXPECT_NE(visited.find(n), visited.end());
	}
}

TEST_F(QueryTest, inline_syntax)
{
	NodeSet nodes;
	Query q = all(*m_graph, nodes);
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(nodes.size(), m_nodes.size());
	for (const auto &n : m_nodes) {
		EXPECT_NE(nodes.find(n), nodes.end());
	}
}
