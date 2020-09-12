#include <procedural_graph/reader/ast_interpreter.h>
#include <procedural_graph/reader/graph_definition_node.h>
#include <procedural_graph/reader/named_argument.h>
#include <procedural_graph/reader/node_definition_node.h>
#include <procedural_graph/reader/node_link_node.h>

#include <procedural_graph/graph.h>

#include <gtest/gtest.h>

using namespace pagoda;

class AstInterpreterTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		m_graph = std::make_shared<Graph>();
		m_interpreter = std::make_shared<AstInterpreter>(m_graph);
	}

	void TearDown() {}

	GraphPtr m_graph;
	std::shared_ptr<AstInterpreter> m_interpreter;
};

TEST_F(AstInterpreterTest, when_visiting_a_named_argument_node_should_create_a_parameter)
{
	NamedArgument n("arg", NamedArgument::ArgumentType::String, "val");
	m_interpreter->Visit(&n);
	ASSERT_EQ(m_interpreter->GetCurrentNamedArguments().size(), 1);
	auto iter = m_interpreter->GetCurrentNamedArguments().begin();
	EXPECT_EQ(iter.first, "arg");
}

TEST_F(AstInterpreterTest, when_visiting_a_string_named_argument_should_create_a_string_parameter)
{
	NamedArgument n("arg", NamedArgument::ArgumentType::String, "val");
	m_interpreter->Visit(&n);
	EXPECT_EQ(get_parameter_as<std::string>(m_interpreter->GetCurrentNamedArguments()["arg"]), "val");
}

TEST_F(AstInterpreterTest, when_visiting_a_float_named_argument_should_create_a_float_parameter)
{
	NamedArgument n("arg", NamedArgument::ArgumentType::Float, "123.0");
	m_interpreter->Visit(&n);
	EXPECT_EQ(get_parameter_as<float>(m_interpreter->GetCurrentNamedArguments()["arg"]), 123.0);
}

TEST_F(AstInterpreterTest, when_visiting_an_expression_named_argument_should_create_an_expression_parameter)
{
	NamedArgument n("arg", NamedArgument::ArgumentType::Expression, "1+1;");
	m_interpreter->Visit(&n);
	EXPECT_NE(std::get<std::shared_ptr<Expression>>(m_interpreter->GetCurrentNamedArguments()["arg"]), nullptr);
}

TEST_F(AstInterpreterTest, when_visiting_a_node_definition_node_should_create_an_entry_in_the_node_table)
{
	NodeDefinitionNode::ConstructionArgumentContainer_t constructionArgs = {
		"interface" : std::make_shared<NamedArgument>("interface", NamedArgument::ArgumentType::String, "out")
	};
	NodeDefinitionNode n("name", "InputInterface", constructionArgs);
	m_interpreter->Visit(&n);
	ASSERT_EQ(m_interpreter->GetNodeTable().size(), 1);
	ASSERT_NE(m_interpreter->GetNodeTable()["name"], nullptr);
}

TEST_F(AstInterpreterTest, when_visiting_a_node_definition_node_should_create_a_node_in_the_graph)
{
	NodeDefinitionNode::ConstructionArgumentContainer_t constructionArgs = {
		"interface" : std::make_shared<NamedArgument>("interface", NamedArgument::ArgumentType::String, "out")
	};
	NodeDefinitionNode n("name", "InputInterface", constructionArgs);
	m_interpreter->Visit(&n);
	ASSERT_EQ(m_graph->GetGraphNodes().size(), 1);
}

TEST_F(AstInterpreterTest, when_visiting_a_node_link_node_should_create_a_link_in_the_graph)
{
	for (auto name : {"n1", "n2"}) {
		NodeDefinitionNode::ConstructionArgumentContainer_t constructionArgs = {
			"interface" : std::make_shared<NamedArgument>("interface", NamedArgument::ArgumentType::String, "out")
		};
		NodeDefinitionNode n(name, "InputInterface", constructionArgs);
		m_interpreter->Visit(&n);
	}

	NodeLinkNode link(NodeLinkNode::NodeLinkContainer_t{"n1", "n2"});
	m_interpreter->Visit(&link);

	auto n1 = m_interpreter->GetNodeTable()["n1"];
	auto n2 = m_interpreter->GetNodeTable()["n2"];
	EXPECT_EQ(m_graph->CreateEdge(n1, n2), Graph::EdgeCreated::EdgeExists);
}
