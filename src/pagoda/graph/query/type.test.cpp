#include "type.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class TypeTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = m_pagoda.CreateGraph();

		m_graph->CreateNode<OperationNode>("n1");
		m_graph->CreateNode<InputInterfaceNode>("n2");
		m_graph->CreateNode<OutputInterfaceNode>("n3");
		m_graph->CreateNode<ParameterNode>("n4");
	}
	void TearDown() {}

	Pagoda m_pagoda;
	GraphPtr m_graph;

	template<class T>
	void testType()
	{
		Type<T> q(*m_graph, [](NodePtr n) { EXPECT_NE(std::dynamic_pointer_cast<T>(n), nullptr); });
		m_graph->ExecuteQuery(q);
	}
};

TEST_F(TypeTest, should_match_nodes_of_a_certain_type)
{
	testType<OperationNode>();
	testType<InputInterfaceNode>();
	testType<OutputInterfaceNode>();
	testType<ParameterNode>();
}
