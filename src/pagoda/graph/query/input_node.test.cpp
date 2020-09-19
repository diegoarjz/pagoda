#include "input_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/operation_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class InputNodeTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = m_pagoda.CreateGraph();

		m_graph->CreateNode<OperationNode>("in1");
		m_graph->CreateNode<OperationNode>("in2");
		m_graph->CreateNode<OperationNode>("node");

		m_graph->CreateEdge("in1", "node");
		m_graph->CreateEdge("in2", "node");
	}
	void TearDown() {}

	Pagoda m_pagoda;
	GraphPtr m_graph;
};

TEST_F(InputNodeTest, should_only_match_input_nodes)
{
	NodeSet nodes;
	InputNode q(*m_graph, nodes);
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(nodes.size(), 2);
	EXPECT_NE(nodes.find(m_graph->GetNode("in1")), nodes.end());
	EXPECT_NE(nodes.find(m_graph->GetNode("in2")), nodes.end());
}
