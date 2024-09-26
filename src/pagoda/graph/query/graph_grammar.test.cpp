#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_node.h>

#include <pagoda/graph/query/adjacent.h>
#include <pagoda/graph/query/and.h>
#include <pagoda/graph/query/graph_grammar.h>
#include <pagoda/graph/query/output_node.h>
#include <pagoda/graph/query/query.h>
#include <pagoda/graph/query/rule.h>
#include <pagoda/graph/query/topology.h>
#include <pagoda/graph/query/type.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>
#include <iostream>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::query;

class GraphGrammarTest : public ::testing::Test
{
	protected:
	void SetUp() override
	{
		m_graph = m_pagoda.CreateGraph();

		m_graph->CreateNode<OperationNode>("op");
		m_graph->CreateNode<OperationNode>("op2");
		m_graph->CreateNode<OperationNode>("op3");
		m_graph->CreateNode<OutputInterfaceNode>("out");
		m_graph->CreateNode<OutputInterfaceNode>("out2");
		m_graph->CreateEdge("op", "out");
		m_graph->CreateEdge("op2", "out2");
	}

	void TearDown() override {}

	Pagoda m_pagoda;
	GraphPtr m_graph;
};

struct MockRule : public Rule
{
	MockRule(Graph& graph) : Rule(graph), m_topology(&m_opQuery), m_count(0)
	{
		m_topology.AddDownstreamQuery(&m_opQuery, &m_outQuery);
	}

	QueryTopology& GetTopology() override { return m_topology; }
	void ApplyMatch(const std::map<Query*, NodePtr>& match) override { m_count += 1; }

	QueryTopology m_topology;
	Type<OperationNode> m_opQuery;
	Type<OutputInterfaceNode> m_outQuery;
	uint32_t m_count;
};

TEST_F(GraphGrammarTest, should_apply_rules)
{
	GraphGrammar g(*m_graph);
	auto r = std::make_shared<MockRule>(*m_graph);
	g.AddRule(r);
	g.Run();
	EXPECT_EQ(r->m_count, 2u);
}

struct MockDeleteRule : public Rule
{
	MockDeleteRule(Graph& graph) : Rule(graph), m_topology(&m_opQuery)
	{
		m_topology.AddDownstreamQuery(&m_opQuery, &m_outQuery);
	}

	QueryTopology& GetTopology() override { return m_topology; }
	void ApplyMatch(const std::map<Query*, NodePtr>& match) override
	{
		LockNode(match.at(&m_opQuery));
		m_graph->DestroyNode(match.at(&m_opQuery)->GetName());
	}

	QueryTopology m_topology;
	Type<OperationNode> m_opQuery;
	Type<OutputInterfaceNode> m_outQuery;
};

TEST_F(GraphGrammarTest, should_be_able_to_modify_the_graph)
{
	auto r = std::make_shared<MockDeleteRule>(*m_graph);
	GraphGrammar g(*m_graph);
	g.AddRule(r);
	g.Run();

	EXPECT_EQ(m_graph->GetNode("op"), nullptr);
	EXPECT_EQ(m_graph->GetNode("op2"), nullptr);
}

