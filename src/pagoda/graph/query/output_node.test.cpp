#include "output_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/operation_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class OutputNodeTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = m_pagoda.CreateGraph();

		m_graph->CreateNode<OperationNode>("out1");
		m_graph->CreateNode<OperationNode>("out2");
		m_graph->CreateNode<OperationNode>("node");

		m_graph->CreateEdge("node", "out1");
		m_graph->CreateEdge("node", "out2");
	}
	void TearDown() {}

	Pagoda m_pagoda;
	GraphPtr m_graph;
};

TEST_F(OutputNodeTest, should_only_match_output_nodes)
{
	NodeSet nodes;
	OutputNode q(*m_graph, nodes);
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(nodes.size(), 2);
	EXPECT_NE(nodes.find(m_graph->GetNode("out1")), nodes.end());
	EXPECT_NE(nodes.find(m_graph->GetNode("out2")), nodes.end());
}

TEST_F(OutputNodeTest, inline_syntax)
{
	NodeSet nodes;
	OutputNode q = output_node(*m_graph, nodes);
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(nodes.size(), 2);
	EXPECT_NE(nodes.find(m_graph->GetNode("out1")), nodes.end());
	EXPECT_NE(nodes.find(m_graph->GetNode("out2")), nodes.end());
}

TEST_F(OutputNodeTest, to_string)
{
	NodeSet nodes;
	OutputNode q = output_node(*m_graph, nodes);
	EXPECT_EQ(q.ToString(), "OutputNode");
}
