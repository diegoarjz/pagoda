#include <pagoda/graph/query/input_node.h>
#include <pagoda/graph/query/output_node.h>
#include <pagoda/graph/query/query.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>

#include <pagoda/pagoda.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;

class GraphSimpleOperationsTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
		input_interface_node = graph->GetNode(graph->CreateNode<InputInterfaceNode>());
		output_interface_node = graph->GetNode(graph->CreateNode<OutputInterfaceNode>());
		operation_node = graph->GetNode(graph->CreateNode<OperationNode>());
	}

	std::shared_ptr<Graph> graph;
	NodePtr input_interface_node;
	NodePtr output_interface_node;
	NodePtr operation_node;
	Pagoda m_pagoda;
};

TEST_F(GraphSimpleOperationsTest, when_creating_nodes_their_id_should_be_incremental)
{
	EXPECT_EQ(input_interface_node->GetId(), 0);
	EXPECT_EQ(output_interface_node->GetId(), 1);
	EXPECT_EQ(operation_node->GetId(), 2);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_a_node_should_remove_it_from_the_graph)
{
	this->graph->DestroyNode(this->operation_node->GetName());

	EXPECT_EQ(this->graph->GetNodeCount(), 2);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_a_node_should_remove_all_of_its_links)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	this->graph->CreateEdge(this->operation_node->GetName(), this->output_interface_node->GetName());
	this->graph->DestroyNode(this->operation_node->GetName());
	EXPECT_EQ(this->graph->GetNodesAdjacentTo(this->input_interface_node->GetName()).size(), 0);
	EXPECT_EQ(this->graph->GetNodesAdjacentTo(this->output_interface_node->GetName()).size(), 0);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_input_node_should_remove_it_from_the_input_nodes)
{
	this->graph->DestroyNode(this->operation_node->GetName());
	NodeSet<Node> inNodes;
    query::InputNode q(inNodes);
    this->graph->ExecuteQuery(q);

	EXPECT_EQ(std::find(inNodes.begin(), inNodes.end(), this->operation_node), std::end(inNodes));
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_output_node_should_remove_it_from_the_output_nodes)
{
	this->graph->DestroyNode(this->operation_node->GetName());
	NodeSet<Node> outNodes;
    query::OutputNode q(outNodes);
    this->graph->ExecuteQuery(q);

	EXPECT_EQ(std::find(outNodes.begin(), outNodes.end(), this->operation_node), std::end(outNodes));
}

TEST_F(GraphSimpleOperationsTest, when_creating_an_edge_should_return_true_created)
{
	EXPECT_EQ(this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName()),
	          Graph::EdgeCreated::Created);
}

TEST_F(GraphSimpleOperationsTest, when_creating_an_edge_between_same_nodes_twice_should_return_edge_exists)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	EXPECT_EQ(this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName()),
	          Graph::EdgeCreated::EdgeExists);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_existing_edge_should_return_edge_destroyed)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	EXPECT_EQ(this->graph->DestroyEdge(this->input_interface_node->GetName(), this->operation_node->GetName()),
	          Graph::EdgeDestroyed::Destroyed);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_non_existing_edge_should_return_edge_doesnt_exist)
{
	EXPECT_EQ(this->graph->DestroyEdge(this->input_interface_node->GetName(), this->operation_node->GetName()),
	          Graph::EdgeDestroyed::EdgeDoesntExist);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_adjacent_nodes_should_return_all_linked_nodes)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	this->graph->CreateEdge(this->operation_node->GetName(), this->output_interface_node->GetName());

	auto all_nodes = this->graph->GetNodesAdjacentTo(this->operation_node->GetName());

	ASSERT_EQ(all_nodes.size(), 2);
	ASSERT_NE(std::find(std::begin(all_nodes), std::end(all_nodes), this->input_interface_node), std::end(all_nodes));
	ASSERT_NE(std::find(std::begin(all_nodes), std::end(all_nodes), this->output_interface_node), std::end(all_nodes));
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_in_adjacent_nodes_should_return_only_the_in_nodes)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	this->graph->CreateEdge(this->operation_node->GetName(), this->output_interface_node->GetName());

	auto in_nodes = this->graph->GetNodeInputNodes(this->operation_node->GetName());

	ASSERT_EQ(in_nodes.size(), 1);
	EXPECT_EQ(*in_nodes.begin(), this->input_interface_node);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_out_adjacent_nodes_should_return_only_the_out_nodes)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	this->graph->CreateEdge(this->operation_node->GetName(), this->output_interface_node->GetName());

	auto out_nodes = this->graph->GetNodeOutputNodes(this->operation_node->GetName());

	ASSERT_EQ(out_nodes.size(), 1);
	EXPECT_EQ(*out_nodes.begin(), this->output_interface_node);
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_nodes_should_remove_links)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	this->graph->DestroyEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	auto all_linked_nodes = this->graph->GetNodesAdjacentTo(this->operation_node->GetName());

	ASSERT_EQ(all_linked_nodes.size(), 0);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_a_graphs_input_nodes_should_return_all_nodes_that_dont_have_inputs)
{
    NodeSet<Node> input_nodes;
    query::InputNode q(input_nodes);
    this->graph->ExecuteQuery(q);

	ASSERT_EQ(input_nodes.size(), 3);
	for (auto n : input_nodes)
	{
		EXPECT_EQ(this->graph->GetNodeInputNodes(n->GetName()).size(), 0);
	}
}

TEST_F(GraphSimpleOperationsTest, when_linking_a_node_to_an_input_node_should_remove_it_from_input_nodes)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
    NodeSet<Node> input_nodes;
    query::InputNode q(input_nodes);
    this->graph->ExecuteQuery(q);

	ASSERT_EQ(input_nodes.size(), 2);
	ASSERT_NE(std::find(input_nodes.begin(), input_nodes.end(), this->input_interface_node), std::end(input_nodes));
	ASSERT_NE(std::find(input_nodes.begin(), input_nodes.end(), this->output_interface_node), std::end(input_nodes));
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_a_node_making_it_an_input_node_should_add_it_to_input_nodes)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	this->graph->DestroyEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
    NodeSet<Node> input_nodes;
    query::InputNode q(input_nodes);
    this->graph->ExecuteQuery(q);

	ASSERT_EQ(input_nodes.size(), 3);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_a_graphs_output_nodes_should_return_all_nodes_that_dont_have_outputs)
{
    NodeSet<Node> output_nodes;
    query::OutputNode q(output_nodes);
    this->graph->ExecuteQuery(q);

	ASSERT_EQ(output_nodes.size(), 3);
	for (auto n : output_nodes)
	{
		EXPECT_EQ(this->graph->GetNodeInputNodes(n->GetName()).size(), 0);
	}
}

TEST_F(GraphSimpleOperationsTest, when_linking_a_node_to_an_output_node_should_remove_it_from_output_nodes)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
    NodeSet<Node> output_nodes;
    query::OutputNode q(output_nodes);
    this->graph->ExecuteQuery(q);

	ASSERT_EQ(output_nodes.size(), 2);
	ASSERT_NE(std::find(output_nodes.begin(), output_nodes.end(), this->output_interface_node), std::end(output_nodes));
	ASSERT_NE(std::find(output_nodes.begin(), output_nodes.end(), this->operation_node), std::end(output_nodes));
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_a_node_making_it_an_output_node_should_add_it_to_output_nodes)
{
	this->graph->CreateEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
	this->graph->DestroyEdge(this->input_interface_node->GetName(), this->operation_node->GetName());
    NodeSet<Node> output_nodes;
    query::OutputNode q(output_nodes);
    this->graph->ExecuteQuery(q);

	ASSERT_EQ(output_nodes.size(), 3);
}

class GraphNodeCreationByName : public ::testing::Test
{
protected:
	virtual void SetUp() { m_graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory()); }

	std::shared_ptr<Graph> m_graph;
	Pagoda m_pagoda;
};

TEST_F(GraphNodeCreationByName, when_creating_nodes_with_name_should_set_the_node_name)
{
	auto node = m_graph->GetNode(m_graph->CreateNode<OperationNode>("operation"));
	EXPECT_EQ(node->GetName(), "operation");
}

TEST_F(GraphNodeCreationByName, when_creating_multiple_nodes_with_the_same_name_should_add_an_index)
{
	auto node1 = m_graph->CreateNode<OperationNode>("operation");
	auto node2 = m_graph->CreateNode<OperationNode>("operation");
	EXPECT_EQ(node1, "operation");
	EXPECT_EQ(node2, "operation1");
}

TEST_F(GraphNodeCreationByName, when_creating_nodes_without_a_name_should_use_node_type_as_default)
{
	auto node = m_graph->CreateNode<OperationNode>();
	EXPECT_EQ(node, OperationNode::name);
}

TEST_F(GraphNodeCreationByName, when_creating_node_by_name_should_be_able_to_get_by_name)
{
	auto node = m_graph->GetNode(m_graph->CreateNode<OperationNode>("operation"));
	EXPECT_EQ(m_graph->GetNode("operation"), node);

	node = m_graph->GetNode(m_graph->CreateNode(InputInterfaceNode::name));
	EXPECT_EQ(m_graph->GetNode(InputInterfaceNode::name), node);
}

