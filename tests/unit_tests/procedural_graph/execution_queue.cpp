#include <procedural_graph/execution_queue.h>
#include <procedural_graph/graph.h>
#include <procedural_graph/operation_node.h>

#include <gtest/gtest.h>

using namespace selector;

class ExecutionQueueTest : public ::testing::Test
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

TEST_F(ExecutionQueueTest, when_iterating_the_nodes_should_visit_them_by_depth)
{
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

	ExecutionQueue q(m_graph);

	ASSERT_EQ(q.GetNodeCount(), 5);
	EXPECT_EQ(q.GetNextNode()->GetId(), 0);
	EXPECT_EQ(q.GetNextNode()->GetId(), 0);
	EXPECT_EQ(q.GetNextNode()->GetId(), 1);
	EXPECT_EQ(q.GetNextNode()->GetId(), 1);
	EXPECT_EQ(q.GetNextNode()->GetId(), 2);
}
