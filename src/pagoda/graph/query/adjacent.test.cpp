#include "adjacent.h"

#include "input_node.h"
#include "output_node.h"
#include "type.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class AdjacentTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = m_pagoda.CreateGraph();

		m_graph->CreateNode<OperationNode>("n1");
		m_graph->CreateNode<OutputInterfaceNode>("n2");
		m_graph->CreateNode<OutputInterfaceNode>("n3");

		m_graph->CreateEdge("n1", "n2");
	}

	void TearDown() {}

	Pagoda m_pagoda;
	GraphPtr m_graph;
	NodeSet m_nodes;
};

TEST_F(AdjacentTest, should_check_downstream_nodes)
{
	Adjacent q{*m_graph, m_nodes, std::make_shared<OutputNode>(), 1};
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(m_nodes.size(), 1);
	EXPECT_NE(m_nodes.find(m_graph->GetNode("n1")), m_nodes.end());
}

TEST_F(AdjacentTest, should_check_upstream_nodes)
{
	Adjacent q{*m_graph, m_nodes, std::make_shared<InputNode>(), -1};
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(m_nodes.size(), 1);
	EXPECT_NE(m_nodes.find(m_graph->GetNode("n2")), m_nodes.end());
}

TEST_F(AdjacentTest, should_check_adjacent_nodes)
{
	m_graph->CreateEdge("n3", "n1");
	Adjacent q{*m_graph, m_nodes, std::make_shared<Type<OutputInterfaceNode>>(), 0};
	m_graph->ExecuteQuery(q);

	EXPECT_EQ(m_nodes.size(), 1);
	EXPECT_NE(m_nodes.find(m_graph->GetNode("n1")), m_nodes.end());
}

TEST_F(AdjacentTest, to_string)
{
	Adjacent q{*m_graph, m_nodes, std::make_shared<OutputNode>(), 1};
	EXPECT_EQ(q.ToString(), "Downstream(1)[OutputNode]");
}
