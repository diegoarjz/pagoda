#include "not.h"

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

class NotTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = m_pagoda.CreateGraph();

		m_graph->CreateNode<ParameterNode>("n1");
		m_graph->CreateNode<OperationNode>("n2");
	}

	void TearDown() {}

	Pagoda m_pagoda;
	GraphPtr m_graph;
	NodeSet m_nodes;
};

TEST_F(NotTest, should_do_an_not_operation)
{
	Not q{*m_graph, m_nodes, std::make_shared<Type<ParameterNode>>()};
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(m_nodes.size(), 1);
	EXPECT_NE(m_nodes.find(m_graph->GetNode("n2")), m_nodes.end());
}

TEST_F(NotTest, inline_syntax)
{
	Not q = ~type<ParameterNode>(*m_graph, m_nodes);
	m_graph->ExecuteQuery(q);
	EXPECT_EQ(m_nodes.size(), 1);
	EXPECT_NE(m_nodes.find(m_graph->GetNode("n2")), m_nodes.end());
}

TEST_F(NotTest, to_string)
{
	Not q = ~type<ParameterNode>(*m_graph, m_nodes);
	EXPECT_EQ(q.ToString(), "Not[Type<Parameter>]");
}
