#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>
#include <pagoda/graph/parameter_node.h>

#include <pagoda/graph/query/adjacent.h>
#include <pagoda/graph/query/and.h>
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

class RuleTest : public ::testing::Test
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
		m_graph->CreateNode<ParameterNode>("parameter");

		m_graph->CreateEdge("op", "out");
		m_graph->CreateEdge("op2", "out2");
	}

	void TearDown() override {}

	Pagoda m_pagoda;
	GraphPtr m_graph;
};

struct MockRule : public Rule
{
	MockRule(Graph& g) : Rule(g), m_topology(&m_opQuery), m_applyCount(0)
	{
		m_topology.AddDownstreamQuery(&m_opQuery, &m_outQuery);
	}

	QueryTopology& GetTopology() override { return m_topology; }
	void ApplyMatch(const std::map<Query*, NodePtr>& match) override
	{
		m_applyCount += 1;
		LockNode(match.at(&m_opQuery));
		LockNode(match.at(&m_outQuery));
	}

	Type<OperationNode> m_opQuery;
	Type<OutputInterfaceNode> m_outQuery;
	QueryTopology m_topology;
	uint32_t m_applyCount;
};

TEST_F(RuleTest, should_be_able_to_create_matches)
{
	MockRule r(*m_graph);
	r.Match();
	auto matches = r.GetMatches();
	ASSERT_EQ(matches.size(), 2);
}

TEST_F(RuleTest, should_be_able_to_apply_the_rule)
{
	MockRule r(*m_graph);
	r.Match();
	r.Apply();
	EXPECT_EQ(r.m_applyCount, 2);
}

TEST_F(RuleTest, should_be_able_to_lock_nodes)
{
	m_graph->CreateEdge("op3", "out2");

	MockRule r(*m_graph);
	r.Match();
	r.Apply();
	auto lockedNodes = r.GetLockedNodes();
	ASSERT_EQ(lockedNodes.size(), 4);
}
