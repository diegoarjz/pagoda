#include "ast_interpreter.h"
#include "graph_definition_node.h"
#include "named_argument.h"
#include "node_definition_node.h"
#include "node_link_definition.h"
#include "node_link_node.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/pagoda.h>

#include <pagoda/dynamic/get_value_as.h>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::io;
using namespace pagoda::dynamic;

class AstInterpreterTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = std::make_shared<Graph>(m_pagoda.GetNodeFactory());
		m_interpreter = std::make_shared<AstInterpreter>(m_graph);
	}

	void TearDown() {}

	GraphPtr m_graph;
	Pagoda m_pagoda;
	std::shared_ptr<AstInterpreter> m_interpreter;
};

TEST_F(AstInterpreterTest, when_visiting_a_named_argument_node_should_create_a_parameter)
{
	NamedArgument n("arg", NamedArgument::ArgumentType::String, "val");
	m_interpreter->Visit(&n);
	ASSERT_EQ(m_interpreter->GetCurrentNamedArguments().size(), 1);
	auto iter = m_interpreter->GetCurrentNamedArguments().begin();
	EXPECT_EQ(iter->first, "arg");
}

TEST_F(AstInterpreterTest, when_visiting_a_string_named_argument_should_create_a_string_parameter)
{
	NamedArgument n("arg", NamedArgument::ArgumentType::String, "val");
	m_interpreter->Visit(&n);
	EXPECT_EQ(get_value_as<std::string>(*m_interpreter->GetCurrentNamedArguments().at("arg")), "val");
}

TEST_F(AstInterpreterTest, when_visiting_a_float_named_argument_should_create_a_float_parameter)
{
	NamedArgument n("arg", NamedArgument::ArgumentType::Float, "123.0");
	m_interpreter->Visit(&n);
	EXPECT_EQ(get_value_as<float>(*m_interpreter->GetCurrentNamedArguments().at("arg")), 123.0);
}

TEST_F(AstInterpreterTest, when_visiting_a_node_definition_node_should_create_a_node_in_the_graph)
{
	NodeDefinitionNode::ConstructionArgumentContainer_t constructionArgs = {
	  std::make_shared<NamedArgument>("interface", NamedArgument::ArgumentType::String, "out")};
	NodeDefinitionNode n("name", "InputInterface", constructionArgs);
	m_interpreter->Visit(&n);
	EXPECT_EQ(m_graph->GetNodeCount(), 1);
	EXPECT_NE(m_graph->GetNode("name"), nullptr);
}

TEST_F(AstInterpreterTest, when_visiting_a_node_link_node_should_create_a_link_in_the_graph)
{
	NodeLinkNode link;  //(NodeLinkNode::NodeLinkContainer_t{"n1", "n2"});
	for (auto name : {"n1", "n2"}) {
		NodeDefinitionNode::ConstructionArgumentContainer_t constructionArgs = {
		  std::make_shared<NamedArgument>("interface", NamedArgument::ArgumentType::String, "out")};
		NodeDefinitionNode n(name, "InputInterface", constructionArgs);
		m_interpreter->Visit(&n);

		link.AddLinkedNode(name);
	}

	m_interpreter->Visit(&link);
	EXPECT_EQ(m_graph->CreateEdge("n1", "n2"), Graph::EdgeCreated::EdgeExists);
}

TEST_F(AstInterpreterTest, when_visiting_a_node_link_definition_should_create_in_out_interface_nodes)
{
	m_graph->CreateNode<OperationNode>("node");
	NodeLinkDefinition def("node", "in", "out");

	m_interpreter->Visit(&def);
	EXPECT_NE(m_graph->GetNode("node_in"), nullptr);
	EXPECT_NE(m_graph->GetNode("node_out"), nullptr);
	EXPECT_EQ(m_graph->CreateEdge("node_in", "node"), Graph::EdgeCreated::EdgeExists);
	EXPECT_EQ(m_graph->CreateEdge("node", "node_out"), Graph::EdgeCreated::EdgeExists);
}
