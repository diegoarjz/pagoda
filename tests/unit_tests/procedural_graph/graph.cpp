#include <procedural_graph/graph.h>
#include <procedural_graph/input_interface_node.h>
#include <procedural_graph/node.h>
#include <procedural_graph/operation_node.h>
#include <procedural_graph/output_interface_node.h>

#include <pagoda.h>

#include <gtest/gtest.h>

#include "mock_objects.h"

using namespace pagoda;

class GraphSimpleOperationsTest : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
		input_interface_node = graph->CreateNode<InputInterfaceNode>();
		output_interface_node = graph->CreateNode<OutputInterfaceNode>();
		operation_node = graph->CreateNode<OperationNode>();
	}

	std::shared_ptr<Graph> graph;
	NodePtr input_interface_node;
	NodePtr output_interface_node;
	NodePtr operation_node;
    Pagoda m_pagoda;
};

TEST_F(GraphSimpleOperationsTest, when_destroying_a_node_should_remove_it_from_the_graph)
{
	this->graph->DestroyNode(this->operation_node);

	EXPECT_EQ(this->graph->GetGraphNodes().size(), 2);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_a_node_should_remove_all_of_its_links)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->CreateEdge(this->operation_node, this->output_interface_node);
	this->graph->DestroyNode(this->operation_node);
	EXPECT_EQ(this->graph->GetNodesAdjacentTo(this->input_interface_node).size(), 0);
	EXPECT_EQ(this->graph->GetNodesAdjacentTo(this->output_interface_node).size(), 0);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_input_node_should_remove_it_from_the_input_nodes)
{
	this->graph->DestroyNode(this->operation_node);
	auto inNodes = this->graph->GetGraphInputNodes();

	EXPECT_EQ(std::find(inNodes.begin(), inNodes.end(), this->operation_node), std::end(inNodes));
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_output_node_should_remove_it_from_the_output_nodes)
{
	this->graph->DestroyNode(this->operation_node);
	auto outNodes = this->graph->GetGraphOutputNodes();

	EXPECT_EQ(std::find(outNodes.begin(), outNodes.end(), this->operation_node), std::end(outNodes));
}

TEST_F(GraphSimpleOperationsTest, when_creating_an_edge_should_return_true_edge_created)
{
	EXPECT_EQ(this->graph->CreateEdge(this->input_interface_node, this->operation_node), Graph::EdgeCreated::Created);
}

TEST_F(GraphSimpleOperationsTest, when_creating_an_edge_between_same_nodes_twice_should_return_edge_exists)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	EXPECT_EQ(this->graph->CreateEdge(this->input_interface_node, this->operation_node),
	          Graph::EdgeCreated::EdgeExists);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_existing_edge_should_return_edge_destroyed)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	EXPECT_EQ(this->graph->DestroyEdge(this->input_interface_node, this->operation_node),
	          Graph::EdgeDestroyed::Destroyed);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_an_non_existing_edge_should_return_edge_doesnt_exist)
{
	EXPECT_EQ(this->graph->DestroyEdge(this->input_interface_node, this->operation_node),
	          Graph::EdgeDestroyed::EdgeDoesntExist);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_adjacent_nodes_should_return_all_linked_nodes)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->CreateEdge(this->operation_node, this->output_interface_node);

	auto all_nodes = this->graph->GetNodesAdjacentTo(this->operation_node);

	ASSERT_EQ(all_nodes.size(), 2);
	ASSERT_NE(std::find(std::begin(all_nodes), std::end(all_nodes), this->input_interface_node), std::end(all_nodes));
	ASSERT_NE(std::find(std::begin(all_nodes), std::end(all_nodes), this->output_interface_node), std::end(all_nodes));
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_in_adjacent_nodes_should_return_only_the_in_nodes)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->CreateEdge(this->operation_node, this->output_interface_node);

	auto in_nodes = this->graph->GetNodeInputNodes(this->operation_node);

	ASSERT_EQ(in_nodes.size(), 1);
	EXPECT_EQ(*in_nodes.begin(), this->input_interface_node);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_out_adjacent_nodes_should_return_only_the_out_nodes)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->CreateEdge(this->operation_node, this->output_interface_node);

	auto out_nodes = this->graph->GetNodeOutputNodes(this->operation_node);

	ASSERT_EQ(out_nodes.size(), 1);
	EXPECT_EQ(*out_nodes.begin(), this->output_interface_node);
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_nodes_should_remove_links)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->DestroyEdge(this->input_interface_node, this->operation_node);
	auto all_linked_nodes = this->graph->GetNodesAdjacentTo(this->operation_node);

	ASSERT_EQ(all_linked_nodes.size(), 0);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_a_graphs_input_nodes_should_return_all_nodes_that_dont_have_inputs)
{
	auto input_nodes = this->graph->GetGraphInputNodes();

	ASSERT_EQ(input_nodes.size(), 3);
	for (auto n : input_nodes)
	{
		EXPECT_EQ(this->graph->GetNodeInputNodes(n).size(), 0);
	}
}

TEST_F(GraphSimpleOperationsTest, when_linking_a_node_to_an_input_node_should_remove_it_from_input_nodes)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	auto input_nodes = this->graph->GetGraphInputNodes();

	ASSERT_EQ(input_nodes.size(), 2);
	ASSERT_NE(std::find(input_nodes.begin(), input_nodes.end(), this->input_interface_node), std::end(input_nodes));
	ASSERT_NE(std::find(input_nodes.begin(), input_nodes.end(), this->output_interface_node), std::end(input_nodes));
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_a_node_making_it_an_input_node_should_add_it_to_input_nodes)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->DestroyEdge(this->input_interface_node, this->operation_node);
	auto input_nodes = this->graph->GetGraphInputNodes();

	ASSERT_EQ(input_nodes.size(), 3);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_a_graphs_output_nodes_should_return_all_nodes_that_dont_have_outputs)
{
	auto output_nodes = this->graph->GetGraphOutputNodes();

	ASSERT_EQ(output_nodes.size(), 3);
	for (auto n : output_nodes)
	{
		EXPECT_EQ(this->graph->GetNodeInputNodes(n).size(), 0);
	}
}

TEST_F(GraphSimpleOperationsTest, when_linking_a_node_to_an_output_node_should_remove_it_from_output_nodes)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	auto output_nodes = this->graph->GetGraphOutputNodes();

	ASSERT_EQ(output_nodes.size(), 2);
	ASSERT_NE(std::find(output_nodes.begin(), output_nodes.end(), this->output_interface_node), std::end(output_nodes));
	ASSERT_NE(std::find(output_nodes.begin(), output_nodes.end(), this->operation_node), std::end(output_nodes));
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_a_node_making_it_an_output_node_should_add_it_to_output_nodes)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->DestroyEdge(this->input_interface_node, this->operation_node);
	auto output_nodes = this->graph->GetGraphOutputNodes();

	ASSERT_EQ(output_nodes.size(), 3);
}
