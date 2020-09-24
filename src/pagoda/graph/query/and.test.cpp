#include "and.h"

#include "input_node.h"
#include "type.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/parameter_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class AndTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = m_pagoda.CreateGraph();

		m_graph->CreateNode<ParameterNode>("n1");
		m_graph->CreateNode<OperationNode>("n2");
		m_graph->CreateNode<InputInterfaceNode>("n3");
		m_graph->CreateNode<ParameterNode>("n4");
		m_graph->CreateEdge("n1", "n2");
		m_graph->CreateEdge("n2", "n4");
	}

	void TearDown() {}

	Pagoda m_pagoda;
	GraphPtr m_graph;
	NodeSet m_nodes;
};

TEST_F(AndTest, should_do_an_and_operation)
{
	And q{*m_graph, m_nodes, {std::make_shared<Type<ParameterNode>>(), std::make_shared<InputNode>()}};
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(m_nodes.size(), 1);
	EXPECT_NE(m_nodes.find(m_graph->GetNode("n1")), m_nodes.end());
}

TEST_F(AndTest, inline_syntax)
{
	And q = type<ParameterNode>(*m_graph, m_nodes) & input_node();
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(m_nodes.size(), 1);
	EXPECT_NE(m_nodes.find(m_graph->GetNode("n1")), m_nodes.end());
}

TEST_F(AndTest, to_string)
{
	And q = type<ParameterNode>(*m_graph, m_nodes) & input_node();
	EXPECT_EQ(q.ToString(),
	          R"(And[
 Type<Parameter>
 InputNode
])");
}
