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

class GraphSimpleOperationsTest : public ::testing::Test {
protected:
  virtual void SetUp() {
    graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
    m_inputInterfaceNode =
        graph->GetNode(graph->CreateNode<InputInterfaceNode>());
    m_outputInterfaceNode =
        graph->GetNode(graph->CreateNode<OutputInterfaceNode>());
    m_operationNode = graph->GetNode(graph->CreateNode<OperationNode>());
  }

  std::shared_ptr<Graph> graph;
  NodePtr m_inputInterfaceNode;
  NodePtr m_outputInterfaceNode;
  NodePtr m_operationNode;
  Pagoda m_pagoda;
};

TEST_F(GraphSimpleOperationsTest,
       when_creating_nodes_their_id_should_be_incremental) {
  EXPECT_EQ(m_inputInterfaceNode->GetId(), 0u);
  EXPECT_EQ(m_outputInterfaceNode->GetId(), 1u);
  EXPECT_EQ(m_operationNode->GetId(), 2u);
}

TEST_F(GraphSimpleOperationsTest,
       when_destroying_a_node_should_remove_it_from_the_graph) {
  this->graph->DestroyNode(this->m_operationNode->GetName());

  EXPECT_EQ(this->graph->GetNodeCount(), 2u);
}

TEST_F(GraphSimpleOperationsTest,
       when_destroying_a_node_should_remove_all_of_its_links) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  this->graph->CreateEdge(this->m_operationNode->GetName(),
                          this->m_outputInterfaceNode->GetName());
  this->graph->DestroyNode(this->m_operationNode->GetName());
  EXPECT_EQ(
      this->graph->GetNodesAdjacentTo(this->m_inputInterfaceNode->GetName())
          .size(),
      0u);
  EXPECT_EQ(
      this->graph->GetNodesAdjacentTo(this->m_outputInterfaceNode->GetName())
          .size(),
      0u);
}

TEST_F(GraphSimpleOperationsTest,
       when_destroying_an_input_node_should_remove_it_from_the_input_nodes) {
  this->graph->DestroyNode(this->m_operationNode->GetName());
  NodeSet inNodes;
  query::InputNode q(*graph, inNodes);
  this->graph->ExecuteQuery(q);

  EXPECT_EQ(std::find(inNodes.begin(), inNodes.end(), this->m_operationNode),
            std::end(inNodes));
}

TEST_F(GraphSimpleOperationsTest,
       when_destroying_an_output_node_should_remove_it_from_the_output_nodes) {
  this->graph->DestroyNode(this->m_operationNode->GetName());
  NodeSet outNodes;
  query::OutputNode q(*graph, outNodes);
  this->graph->ExecuteQuery(q);

  EXPECT_EQ(std::find(outNodes.begin(), outNodes.end(), this->m_operationNode),
            std::end(outNodes));
}

TEST_F(GraphSimpleOperationsTest,
       when_creating_an_edge_should_return_true_created) {
  EXPECT_EQ(this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                                    this->m_operationNode->GetName()),
            Graph::EdgeCreated::Created);
}

TEST_F(
    GraphSimpleOperationsTest,
    when_creating_an_edge_between_same_nodes_twice_should_return_edge_exists) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  EXPECT_EQ(this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                                    this->m_operationNode->GetName()),
            Graph::EdgeCreated::EdgeExists);
}

TEST_F(GraphSimpleOperationsTest,
       when_destroying_an_existing_edge_should_return_edge_destroyed) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  EXPECT_EQ(this->graph->DestroyEdge(this->m_inputInterfaceNode->GetName(),
                                     this->m_operationNode->GetName()),
            Graph::EdgeDestroyed::Destroyed);
}

TEST_F(GraphSimpleOperationsTest,
       when_destroying_an_non_existing_edge_should_return_edge_doesnt_exist) {
  EXPECT_EQ(this->graph->DestroyEdge(this->m_inputInterfaceNode->GetName(),
                                     this->m_operationNode->GetName()),
            Graph::EdgeDestroyed::EdgeDoesntExist);
}

TEST_F(GraphSimpleOperationsTest,
       when_retrieving_adjacent_nodes_should_return_all_linked_nodes) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  this->graph->CreateEdge(this->m_operationNode->GetName(),
                          this->m_outputInterfaceNode->GetName());

  auto all_nodes =
      this->graph->GetNodesAdjacentTo(this->m_operationNode->GetName());

  ASSERT_EQ(all_nodes.size(), 2u);
  ASSERT_NE(std::find(std::begin(all_nodes), std::end(all_nodes),
                      this->m_inputInterfaceNode),
            std::end(all_nodes));
  ASSERT_NE(std::find(std::begin(all_nodes), std::end(all_nodes),
                      this->m_outputInterfaceNode),
            std::end(all_nodes));
}

TEST_F(GraphSimpleOperationsTest,
       when_retrieving_in_adjacent_nodes_should_return_only_the_in_nodes) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  this->graph->CreateEdge(this->m_operationNode->GetName(),
                          this->m_outputInterfaceNode->GetName());

  auto in_nodes =
      this->graph->GetNodeInputNodes(this->m_operationNode->GetName());

  ASSERT_EQ(in_nodes.size(), 1u);
  EXPECT_EQ(*in_nodes.begin(), this->m_inputInterfaceNode);
}

TEST_F(GraphSimpleOperationsTest,
       when_retrieving_out_adjacent_nodes_should_return_only_the_out_nodes) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  this->graph->CreateEdge(this->m_operationNode->GetName(),
                          this->m_outputInterfaceNode->GetName());

  auto out_nodes =
      this->graph->GetNodeOutputNodes(this->m_operationNode->GetName());

  ASSERT_EQ(out_nodes.size(), 1u);
  EXPECT_EQ(*out_nodes.begin(), this->m_outputInterfaceNode);
}

TEST_F(GraphSimpleOperationsTest, when_unlinking_nodes_should_remove_links) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  this->graph->DestroyEdge(this->m_inputInterfaceNode->GetName(),
                           this->m_operationNode->GetName());
  auto all_linked_nodes =
      this->graph->GetNodesAdjacentTo(this->m_operationNode->GetName());

  ASSERT_EQ(all_linked_nodes.size(), 0u);
}

TEST_F(
    GraphSimpleOperationsTest,
    when_retrieving_a_graphs_input_nodes_should_return_all_nodes_that_dont_have_inputs) {
  NodeSet input_nodes;
  query::InputNode q(*graph, input_nodes);
  this->graph->ExecuteQuery(q);

  ASSERT_EQ(input_nodes.size(), 3u);
  for (auto n : input_nodes) {
    EXPECT_EQ(this->graph->GetNodeInputNodes(n->GetName()).size(), 0u);
  }
}

TEST_F(GraphSimpleOperationsTest,
       when_linking_a_node_to_an_input_node_should_remove_it_from_input_nodes) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  NodeSet input_nodes;
  query::InputNode q(*graph, input_nodes);
  this->graph->ExecuteQuery(q);

  ASSERT_EQ(input_nodes.size(), 2u);
  ASSERT_NE(std::find(input_nodes.begin(), input_nodes.end(),
                      this->m_inputInterfaceNode),
            std::end(input_nodes));
  ASSERT_NE(std::find(input_nodes.begin(), input_nodes.end(),
                      this->m_outputInterfaceNode),
            std::end(input_nodes));
}

TEST_F(
    GraphSimpleOperationsTest,
    when_unlinking_a_node_making_it_an_input_node_should_add_it_to_input_nodes) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  this->graph->DestroyEdge(this->m_inputInterfaceNode->GetName(),
                           this->m_operationNode->GetName());
  NodeSet input_nodes;
  query::InputNode q(*graph, input_nodes);
  this->graph->ExecuteQuery(q);

  ASSERT_EQ(input_nodes.size(), 3u);
}

TEST_F(
    GraphSimpleOperationsTest,
    when_retrieving_a_graphs_output_nodes_should_return_all_nodes_that_dont_have_outputs) {
  NodeSet output_nodes;
  query::OutputNode q(*graph, output_nodes);
  this->graph->ExecuteQuery(q);

  ASSERT_EQ(output_nodes.size(), 3u);
  for (auto n : output_nodes) {
    EXPECT_EQ(this->graph->GetNodeInputNodes(n->GetName()).size(), 0u);
  }
}

TEST_F(
    GraphSimpleOperationsTest,
    when_linking_a_node_to_an_output_node_should_remove_it_from_output_nodes) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  NodeSet output_nodes;
  query::OutputNode q(*graph, output_nodes);
  this->graph->ExecuteQuery(q);

  ASSERT_EQ(output_nodes.size(), 2u);
  ASSERT_NE(std::find(output_nodes.begin(), output_nodes.end(),
                      this->m_outputInterfaceNode),
            std::end(output_nodes));
  ASSERT_NE(
      std::find(output_nodes.begin(), output_nodes.end(), this->m_operationNode),
      std::end(output_nodes));
}

TEST_F(
    GraphSimpleOperationsTest,
    when_unlinking_a_node_making_it_an_output_node_should_add_it_to_output_nodes) {
  this->graph->CreateEdge(this->m_inputInterfaceNode->GetName(),
                          this->m_operationNode->GetName());
  this->graph->DestroyEdge(this->m_inputInterfaceNode->GetName(),
                           this->m_operationNode->GetName());
  NodeSet output_nodes;
  query::OutputNode q(*graph, output_nodes);
  this->graph->ExecuteQuery(q);

  ASSERT_EQ(output_nodes.size(), 3u);
}

class GraphNodeCreationByName : public ::testing::Test {
protected:
  virtual void SetUp() {
    m_graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
  }

  std::shared_ptr<Graph> m_graph;
  Pagoda m_pagoda;
};

TEST_F(GraphNodeCreationByName,
       when_creating_nodes_with_name_should_set_the_node_name) {
  auto node = m_graph->GetNode(m_graph->CreateNode<OperationNode>("operation"));
  EXPECT_EQ(node->GetName(), "operation");
}

TEST_F(GraphNodeCreationByName,
       when_creating_multiple_nodes_with_the_same_name_should_add_an_index) {
  auto node1 = m_graph->CreateNode<OperationNode>("operation");
  auto node2 = m_graph->CreateNode<OperationNode>("operation");
  EXPECT_EQ(node1, "operation");
  EXPECT_EQ(node2, "operation1");
}

TEST_F(GraphNodeCreationByName,
       when_creating_nodes_without_a_name_should_use_node_type_as_default) {
  auto node = m_graph->CreateNode<OperationNode>();
  EXPECT_EQ(node, OperationNode::name);
}

TEST_F(GraphNodeCreationByName,
       when_creating_node_by_name_should_be_able_to_get_by_name) {
  auto node = m_graph->GetNode(m_graph->CreateNode<OperationNode>("operation"));
  EXPECT_EQ(m_graph->GetNode("operation"), node);

  node = m_graph->GetNode(m_graph->CreateNode(InputInterfaceNode::name));
  EXPECT_EQ(m_graph->GetNode(InputInterfaceNode::name), node);
}

class GraphNodeRenaming : public ::testing::Test {
protected:
  virtual void SetUp() {
    m_graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
    m_inputInterfaceNode =
        m_graph->GetNode(m_graph->CreateNode<InputInterfaceNode>());
    m_outputInterfaceNode =
        m_graph->GetNode(m_graph->CreateNode<OutputInterfaceNode>());
    m_operationNode = m_graph->GetNode(m_graph->CreateNode<OperationNode>());
  }

  std::shared_ptr<Graph> m_graph;
  NodePtr m_inputInterfaceNode;
  NodePtr m_outputInterfaceNode;
  NodePtr m_operationNode;
  Pagoda m_pagoda;
};

TEST_F(GraphNodeRenaming, test_rename_without_adjacencies)
{
  const auto opNodeName = m_operationNode->GetName();
  m_graph->RenameNode(opNodeName, "newName");
  EXPECT_EQ(m_operationNode->GetName(), "newName");
}
