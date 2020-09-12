#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/traversal/forward.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
class ForwardTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>("a")));
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>("b")));
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>("c")));
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>("d")));
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>("e")));
	}

	void TearDown() {}

	GraphPtr m_graph;
	std::vector<NodePtr> m_nodes;
	Pagoda m_pagoda;
};

TEST_F(ForwardTest, when_visiting_should_call_the_delegate_for_each_node)
{
	std::set<NodePtr> visitedNodes;
	traversal::Forward forward(*m_graph);
	forward.ForEach([&visitedNodes](NodePtr n) { visitedNodes.insert(n); });
	for (auto n : m_nodes) {
		EXPECT_NE(std::find(visitedNodes.begin(), visitedNodes.end(), n), visitedNodes.end());
	}
}

TEST_F(ForwardTest, when_visiting_should_visit_the_nodes_in_breadth_first_order)
{
	/*
	 * a -> b -> c
	 * a -> d
	 * e
	 */
	m_graph->CreateEdge("a", "b");
	m_graph->CreateEdge("b", "c");
	m_graph->CreateEdge("a", "d");

	std::vector<NodePtr> visitedNodes;
	traversal::Forward forward(*m_graph);
	forward.ForEach([&visitedNodes](NodePtr n) { visitedNodes.push_back(n); });
	EXPECT_EQ(visitedNodes[0]->GetName(), "a");
	EXPECT_EQ(visitedNodes[1]->GetName(), "e");
	EXPECT_EQ(visitedNodes[2]->GetName(), "b");
	EXPECT_EQ(visitedNodes[3]->GetName(), "d");
	EXPECT_EQ(visitedNodes[4]->GetName(), "c");
}
