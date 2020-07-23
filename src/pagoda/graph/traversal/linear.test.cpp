#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/traversal/linear.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
class LinearTest : public ::testing::Test
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

TEST_F(LinearTest, when_visiting_should_call_the_delegate_for_each_node)
{
	std::set<NodePtr> visitedNodes;
	traversal::Linear linear(*m_graph);
	linear.ForEach([&visitedNodes](NodePtr n) { visitedNodes.insert(n); });
	for (auto n : m_nodes)
	{
		EXPECT_NE(std::find(visitedNodes.begin(), visitedNodes.end(), n), visitedNodes.end());
	}
}
