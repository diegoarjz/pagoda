#include <pagoda/graph/execution_queue.h>
#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;

class ExecutionQueueTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
		a = m_graph->GetNode(m_graph->CreateNode<InputInterfaceNode>("a"));
		b = m_graph->GetNode(m_graph->CreateNode<InputInterfaceNode>("b"));
		c = m_graph->GetNode(m_graph->CreateNode<InputInterfaceNode>("c"));
		d = m_graph->GetNode(m_graph->CreateNode<InputInterfaceNode>("d"));
		e = m_graph->GetNode(m_graph->CreateNode<InputInterfaceNode>("e"));
	}

	void TearDown() {}

	GraphPtr m_graph;
	NodePtr a;
	NodePtr b;
	NodePtr c;
	NodePtr d;
	NodePtr e;
	Pagoda m_pagoda;
};

TEST_F(ExecutionQueueTest, when_iterating_the_nodes_should_visit_them_by_depth)
{
	/*
	 * a -> b -> c
	 * a -> d
	 * e
	 */
	a->SetName("a");
	b->SetName("b");
	c->SetName("c");
	d->SetName("d");
	e->SetName("e");
	m_graph->CreateEdge("a", "b");
	m_graph->CreateEdge("b", "c");
	m_graph->CreateEdge("a", "d");

	ExecutionQueue q(*m_graph);

	ASSERT_EQ(q.GetNodeCount(), 5);
	EXPECT_EQ(q.GetNextNode()->GetName(), "a");
	EXPECT_EQ(q.GetNextNode()->GetName(), "e");
	EXPECT_EQ(q.GetNextNode()->GetName(), "b");
	EXPECT_EQ(q.GetNextNode()->GetName(), "d");
	EXPECT_EQ(q.GetNextNode()->GetName(), "c");
}

TEST_F(ExecutionQueueTest, when_iterating_the_nodes_should_only_visit_them_once)
{
	/*
	 * a -> b
	 * a -> c
	 * b -> d
	 * c -> d
	 * d -> e
	 */

	m_graph->CreateEdge("a", "b");
	m_graph->CreateEdge("a", "c");
	m_graph->CreateEdge("b", "d");
	m_graph->CreateEdge("c", "d");
	m_graph->CreateEdge("d", "e");

	ExecutionQueue q(*m_graph);

	std::unordered_set<NodePtr> visitedNodes;
	ASSERT_EQ(q.GetNodeCount(), 5);

	NodePtr n = q.GetNextNode();

	while (n != nullptr) {
		ASSERT_EQ(visitedNodes.find(n), std::end(visitedNodes));
		visitedNodes.insert(n);
		n = q.GetNextNode();
	}
}
