#include <procedural_graph/graph.h>
#include <procedural_graph/node.h>
#include <procedural_graph/node_factory.h>
#include <procedural_graph/node_type.h>

#include <gtest/gtest.h>

#include "mock_objects.h"

using namespace selector;

class GraphCreateNodeTest : public ::testing::TestWithParam<NodeType>
{
protected:
	virtual void SetUp()
	{
		nodeFactory = std::make_shared<MockNodeFactory>();
		graph = std::make_shared<Graph>(nodeFactory);
	}

	virtual void TearDown() {}

	std::shared_ptr<Graph> graph;
	std::shared_ptr<MockNodeFactory> nodeFactory;
};

TEST_P(GraphCreateNodeTest, when_creating_a_valid_node_should_return_a_node_with_the_same_type)
{
	// clang-format off
    EXPECT_CALL(*this->nodeFactory, Create(GetParam(), ""))
        .Times(1)
        .WillOnce(::testing::Return(std::make_shared<Node>(GetParam())));
    // clant-format on
    NodePtr node = this->graph->CreateNode(GetParam());

    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node->GetNodeType(), GetParam());
}

TEST_P(GraphCreateNodeTest, when_creating_nodes_should_create_nodes_with_incremental_ids)
{
    // clang-format off
    EXPECT_CALL(*this->nodeFactory, Create(GetParam(), ""))
        .Times(2)
        .WillRepeatedly(::testing::Return(std::make_shared<Node>(GetParam())));
    // clant-format on
    auto node1 = this->graph->CreateNode(GetParam());
    EXPECT_EQ(node1->GetId(), 0);
    auto node2 = this->graph->CreateNode(GetParam());
    EXPECT_EQ(node2->GetId(), 1);
}

TEST_P(GraphCreateNodeTest, when_creating_nodes_after_destroying_nodes_should_still_create_nodes)
{
    // clang-format off
    EXPECT_CALL(*this->nodeFactory, Create(GetParam(), ""))
        .Times(2)
        .WillRepeatedly(::testing::Return(std::make_shared<Node>(GetParam())));
    // clant-format on
    auto node1 = this->graph->CreateNode(GetParam());
    EXPECT_EQ(node1->GetId(), 0);
    this->graph->DestroyNode(node1);
    auto node2 = this->graph->CreateNode(GetParam());
    EXPECT_EQ(node2->GetId(), 1);
}

INSTANTIATE_TEST_CASE_P(BuiltInNodeTypes,
                        GraphCreateNodeTest,
                        ::testing::Values(NodeType::InputInterface, NodeType::Operation, NodeType::OutputInterface));

class GraphSimpleOperationsTest : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        nodeFactory = std::make_shared<MockNodeFactory>();

        // clang-format off
        EXPECT_CALL(*this->nodeFactory, Create(NodeType::InputInterface, ""))
            .Times(1)
            .WillOnce(::testing::Return(std::make_shared<Node>(NodeType::InputInterface)));
        EXPECT_CALL(*this->nodeFactory, Create(NodeType::OutputInterface, ""))
            .Times(1)
            .WillOnce(::testing::Return(std::make_shared<Node>(NodeType::OutputInterface)));
        EXPECT_CALL(*this->nodeFactory, Create(NodeType::Operation, ""))
            .Times(1)
            .WillOnce(::testing::Return(std::make_shared<Node>(NodeType::Operation)));
        // clant-format on

        graph = std::make_shared<Graph>(nodeFactory);
        input_interface_node = graph->CreateNode(NodeType::InputInterface);
        output_interface_node = graph->CreateNode(NodeType::OutputInterface);
        operation_node = graph->CreateNode(NodeType::Operation);
    }

    std::shared_ptr<Graph> graph;
    NodePtr input_interface_node;
    NodePtr output_interface_node;
    NodePtr operation_node;
    std::shared_ptr<MockNodeFactory> nodeFactory;
};

TEST_F(GraphSimpleOperationsTest, when_getting_nodes_by_type_should_return_nodes_that_match_the_type)
{
    auto interface_nodes = this->graph->GetGraphNodesByType(NodeType::InputInterface);
    auto operation_nodes = this->graph->GetGraphNodesByType(NodeType::Operation);

    EXPECT_EQ(interface_nodes.size(), 1);
    EXPECT_EQ(operation_nodes.size(), 1);

    EXPECT_EQ(*interface_nodes.begin(), this->input_interface_node);
    EXPECT_EQ(*operation_nodes.begin(), this->operation_node);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_a_node_should_remove_it_from_the_graph)
{
    this->graph->DestroyNode(this->operation_node);

    EXPECT_EQ(this->graph->GetGraphNodesByType(NodeType::Operation).size(), 0);
}

TEST_F(GraphSimpleOperationsTest, when_destroying_a_node_should_remove_all_of_its_links)
{
    this->graph->CreateEdge(this->input_interface_node, this->operation_node);
    this->graph->CreateEdge(this->operation_node, this->output_interface_node);
    this->graph->DestroyNode(this->operation_node);
    EXPECT_EQ(this->graph->GetNodesLinkedTo(this->input_interface_node).size(), 0);
    EXPECT_EQ(this->graph->GetNodesLinkedTo(this->output_interface_node).size(), 0);
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

    auto all_nodes = this->graph->GetNodesLinkedTo(this->operation_node);

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
    EXPECT_EQ(in_nodes.front(), this->input_interface_node);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_out_adjacent_nodes_should_return_only_the_out_nodes)
{
    this->graph->CreateEdge(this->input_interface_node, this->operation_node);
    this->graph->CreateEdge(this->operation_node, this->output_interface_node);

    auto out_nodes = this->graph->GetNodeOutputNodes(this->operation_node);

    ASSERT_EQ(out_nodes.size(), 1);
    EXPECT_EQ(out_nodes.front(), this->output_interface_node);
}

TEST_F(GraphSimpleOperationsTest,
when_retrieving_in_adjacent_nodes_of_a_given_type_should_return_only_the_nodes_with_such_type)
{
    // clang-format off
    EXPECT_CALL(*this->nodeFactory, Create(NodeType::Operation, ""))
        .Times(1)
        .WillOnce(::testing::Return(std::make_shared<Node>(NodeType::Operation)));
	// clang-format on
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->CreateEdge(this->graph->CreateNode(NodeType::Operation), this->operation_node);
	this->graph->CreateEdge(this->operation_node, this->output_interface_node);

	auto in_nodes = this->graph->GetNodeInputNodesByType(this->operation_node, NodeType::InputInterface);

	ASSERT_EQ(in_nodes.size(), 1);
	EXPECT_EQ(in_nodes.front(), this->input_interface_node);
}

TEST_F(GraphSimpleOperationsTest,
       when_retrieving_out_adjacent_nodes_of_a_given_type_should_return_only_the_nodes_with_such_type)
{
	// clang-format off
    EXPECT_CALL(*this->nodeFactory, Create(NodeType::Operation, ""))
        .Times(1)
        .WillOnce(::testing::Return(std::make_shared<Node>(NodeType::Operation)));
	// clang-format on
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->CreateEdge(this->graph->CreateNode(NodeType::Operation), this->operation_node);
	this->graph->CreateEdge(this->operation_node, this->output_interface_node);

	auto in_nodes = this->graph->GetNodeOutputNodesByType(this->input_interface_node, NodeType::Operation);

	ASSERT_EQ(in_nodes.size(), 1);
	EXPECT_EQ(in_nodes.front(), this->operation_node);
}

TEST_F(GraphSimpleOperationsTest, when_retrieving_all_adjacent_nodes_of_a_type_should_return_the_nodes_with_such_type)
{
	// clang-format off
    EXPECT_CALL(*this->nodeFactory, Create(NodeType::Operation, ""))
        .Times(2)
        .WillRepeatedly(::testing::Return(std::make_shared<Node>(NodeType::Operation)));
	// clang-format on
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->CreateEdge(this->graph->CreateNode(NodeType::Operation), this->operation_node);
	this->graph->CreateEdge(this->operation_node, this->graph->CreateNode(NodeType::Operation));
	this->graph->CreateEdge(this->operation_node, this->output_interface_node);

	auto all_linked_nodes = this->graph->GetNodesLinkedToByType(this->operation_node, NodeType::Operation);

	ASSERT_EQ(all_linked_nodes.size(), 2);
	for (auto n : all_linked_nodes)
	{
		EXPECT_EQ(n->GetNodeType(), NodeType::Operation);
	}
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_nodes_should_remove_links)
{
	this->graph->CreateEdge(this->input_interface_node, this->operation_node);
	this->graph->DestroyEdge(this->input_interface_node, this->operation_node);
	auto all_linked_nodes = this->graph->GetNodesLinkedTo(this->operation_node);

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
