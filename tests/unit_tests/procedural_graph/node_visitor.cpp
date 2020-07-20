#include <pagoda/graph/breadth_first_node_visitor.h>
#include <pagoda/graph/graph.h>
#include <pagoda/graph/graph_node_visitor.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;

class Delegate
{
public:
	void operator()(NodePtr n) { m_visitedNodes.push_back(n); }

	std::vector<NodePtr> m_visitedNodes;
};

class NodeVisitorTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		m_graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>()));
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>()));
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>()));
		m_nodes.push_back(m_graph->GetNode(m_graph->CreateNode<OperationNode>()));
	}

	void TearDown() {}

	GraphPtr m_graph;
	std::vector<NodePtr> m_nodes;
	Pagoda m_pagoda;
};

TEST_F(NodeVisitorTest, when_visiting_should_call_the_delegate_for_each_node)
{
	Delegate d;
	GraphNodeVisitor visitor(*m_graph, d);
	visitor.Visit();
	for (auto n : m_nodes)
	{
		EXPECT_NE(std::find(d.m_visitedNodes.begin(), d.m_visitedNodes.end(), n), d.m_visitedNodes.end());
	}
}

class BreadthFirstNodeVisitorTest : public ::testing::Test
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

TEST_F(BreadthFirstNodeVisitorTest, when_visiting_should_call_the_delegate_for_each_node)
{
	Delegate d;
	GraphNodeVisitor visitor(*m_graph, d);
	visitor.Visit();
	for (auto n : m_nodes)
	{
		EXPECT_NE(std::find(d.m_visitedNodes.begin(), d.m_visitedNodes.end(), n), d.m_visitedNodes.end());
	}
}

TEST_F(BreadthFirstNodeVisitorTest, when_visiting_should_visit_the_nodes_in_breadth_first_order)
{
	Delegate d;
	/*
	 * a -> b -> c
	 * a -> d
	 * e
	 */
	m_graph->CreateEdge("a", "b");
	m_graph->CreateEdge("b", "c");
	m_graph->CreateEdge("a", "d");

	BreadthFirstNodeVisitor v(*m_graph, d);
	v.Visit();

	EXPECT_EQ(d.m_visitedNodes[0]->GetName(), "a");
	EXPECT_EQ(d.m_visitedNodes[1]->GetName(), "e");
	EXPECT_EQ(d.m_visitedNodes[2]->GetName(), "b");
	EXPECT_EQ(d.m_visitedNodes[3]->GetName(), "d");
	EXPECT_EQ(d.m_visitedNodes[4]->GetName(), "c");
}
