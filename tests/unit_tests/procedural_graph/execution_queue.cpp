#include <procedural_graph/execution_queue.h>
#include <procedural_graph/graph.h>
#include <procedural_graph/input_interface_node.h>

#include <selector.h>

#include <gtest/gtest.h>

using namespace selector;

class ExecutionQueueTest : public ::testing::Test
{
protected:
	void SetUp()
	{
        m_graph = std::make_shared<Graph>(m_selector.GetNodeFactory());
		a = m_graph->CreateNode<InputInterfaceNode>();
		b = m_graph->CreateNode<InputInterfaceNode>();
		c = m_graph->CreateNode<InputInterfaceNode>();
		d = m_graph->CreateNode<InputInterfaceNode>();
		e = m_graph->CreateNode<InputInterfaceNode>();
	}

	void TearDown() {}

	GraphPtr m_graph;
	NodePtr a;
	NodePtr b;
	NodePtr c;
	NodePtr d;
	NodePtr e;
    Selector m_selector;
};

TEST_F(ExecutionQueueTest, when_iterating_the_nodes_should_visit_them_by_depth)
{
	/*
	 * We're going to abuse the Node ids and use it as the depth level
	 * a -> b -> c
	 * a -> d
	 * e
	 */
	a->SetId(0);
	b->SetId(1);
	c->SetId(2);
	d->SetId(1);
	e->SetId(0);
	m_graph->CreateEdge(a, b);
	m_graph->CreateEdge(b, c);
	m_graph->CreateEdge(a, d);

	ExecutionQueue q(*m_graph);

	ASSERT_EQ(q.GetNodeCount(), 5);
	EXPECT_EQ(q.GetNextNode()->GetId(), 0);
	EXPECT_EQ(q.GetNextNode()->GetId(), 0);
	EXPECT_EQ(q.GetNextNode()->GetId(), 1);
	EXPECT_EQ(q.GetNextNode()->GetId(), 1);
	EXPECT_EQ(q.GetNextNode()->GetId(), 2);
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
	a->SetId(0);
	b->SetId(1);
	c->SetId(2);
	d->SetId(1);
	e->SetId(0);

	m_graph->CreateEdge(a, b);
	m_graph->CreateEdge(a, c);
	m_graph->CreateEdge(b, d);
	m_graph->CreateEdge(c, d);
	m_graph->CreateEdge(d, e);

	ExecutionQueue q(*m_graph);

	std::unordered_set<NodePtr> visitedNodes;
	ASSERT_EQ(q.GetNodeCount(), 5);

	NodePtr n = q.GetNextNode();

	while (n != nullptr)
	{
		ASSERT_EQ(visitedNodes.find(n), std::end(visitedNodes));
		visitedNodes.insert(n);
		n = q.GetNextNode();
	}
}
