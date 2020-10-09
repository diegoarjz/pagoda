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

struct Matched1
{
	NodePtr n = nullptr;
	template<class T>
	std::size_t GetLockedNodes(T inserter)
	{
		return 0;
	}
};
TEST_F(RuleTest, a_rule_should_fill_the_match_variable)
{
	Matched1 matched;
	auto q = (type<ParameterNode>() >> matched.n);
	Rule<Matched1> r(*m_graph, q, matched);
	r.Run([&](Matched1 &m) {});
	ASSERT_NE(matched.n, nullptr);
	EXPECT_EQ(matched.n->GetName(), "parameter");
}

TEST_F(RuleTest, a_rule_should_produce_multiple_full_matches)
{
	Matched1 matched;
	auto q = (type<OperationNode>() >> matched.n);
	Rule<Matched1> r(*m_graph, q, matched);

	std::list<Matched1> matches;
	r.Run([&](Matched1 &m) { matches.push_back(m); });
	EXPECT_EQ(matches.size(), 3);
	for (auto n : matches) {
		EXPECT_TRUE(n.n->GetName() == "op" || n.n->GetName() == "op2" || n.n->GetName() == "op3");
	}
}

struct Matched2
{
	NodePtr m_operation;
	NodePtr m_outNode;

	template<class T>
	std::size_t GetLockedNodes(T inserter)
	{
		inserter = m_operation;
		inserter = m_outNode;
		return 2;
	}
};

TEST_F(RuleTest, a_rule_should_produce_multiple_matches_with_different_nodes)
{
	Matched2 matched;
	// clang-format off
  auto q = (type<OperationNode>() >> matched.m_operation) &
           downstream(output_node() >> matched.m_outNode);
	// clang-format on
	Rule<Matched2> r(*m_graph, q, matched);

	std::vector<Matched2> matches;
	r.Run([&](Matched2 &m) { matches.push_back(m); });

	EXPECT_EQ(matches.size(), 2);
	EXPECT_EQ(matches[0].m_operation->GetName(), "op");
	EXPECT_EQ(matches[0].m_outNode->GetName(), "out");
	EXPECT_EQ(matches[1].m_operation->GetName(), "op2");
	EXPECT_EQ(matches[1].m_outNode->GetName(), "out2");
}

TEST_F(RuleTest, a_rule_be_able_to_lock_nodes)
{
	Matched2 matched;
	// clang-format off
  auto q = (type<OperationNode>() >> matched.m_operation) &
           downstream(output_node() >> matched.m_outNode);
	// clang-format on
	Rule<Matched2> r(*m_graph, q, matched);
	r.Run([&](Matched2 &m) {});
	EXPECT_EQ(r.GetLockedNodes().size(), 4);

	for (const auto &n : r.GetLockedNodes()) {
		EXPECT_TRUE(n->GetName() == "op" || n->GetName() == "op2" || n->GetName() == "out" || n->GetName() == "out2");
	}
}
