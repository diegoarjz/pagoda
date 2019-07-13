#include <procedural_graph/breadth_first_node_visitor.h>
#include <procedural_graph/graph.h>
#include <procedural_graph/node.h>
#include <procedural_graph/node_visitor.h>
#include <procedural_graph/operation_node.h>

#include <gtest/gtest.h>

using namespace selector;

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
		m_graph = std::make_shared<Graph>();
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
	}

	void TearDown() {}

	GraphPtr m_graph;
	std::vector<NodePtr> m_nodes;
};

TEST_F(NodeVisitorTest, when_visiting_should_call_the_delegate_for_each_node)
{
	Delegate d;
	NodeVisitor visitor(m_graph, d);
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
		m_graph = std::make_shared<Graph>();
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
		m_nodes.push_back(m_graph->CreateNode<OperationNode>());
	}

	void TearDown() {}

	GraphPtr m_graph;
	std::vector<NodePtr> m_nodes;
};

TEST_F(BreadthFirstNodeVisitorTest, when_visiting_should_call_the_delegate_for_each_node)
{
	Delegate d;
	NodeVisitor visitor(m_graph, d);
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
	 * We're going to abuse the Node ids and use it as the depth level
	 * a -> b -> c
	 * a -> d
	 * e
	 */
	m_nodes[0]->SetId(0);
	m_nodes[1]->SetId(1);
	m_nodes[2]->SetId(2);
	m_nodes[3]->SetId(1);
	m_nodes[4]->SetId(0);
	m_graph->CreateEdge(m_nodes[0], m_nodes[1]);
	m_graph->CreateEdge(m_nodes[1], m_nodes[2]);
	m_graph->CreateEdge(m_nodes[0], m_nodes[3]);

	BreadthFirstNodeVisitor v(m_graph, d);
	v.Visit();

	EXPECT_EQ(d.m_visitedNodes[0]->GetId(), 0);
	EXPECT_EQ(d.m_visitedNodes[1]->GetId(), 0);
	EXPECT_EQ(d.m_visitedNodes[2]->GetId(), 1);
	EXPECT_EQ(d.m_visitedNodes[3]->GetId(), 1);
	EXPECT_EQ(d.m_visitedNodes[4]->GetId(), 2);
}
