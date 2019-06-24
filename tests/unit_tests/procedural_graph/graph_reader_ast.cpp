#include <procedural_graph/reader/graph_definition_node.h>
#include <procedural_graph/reader/named_argument.h>
#include <procedural_graph/reader/node_definition_node.h>
#include <procedural_graph/reader/node_link_node.h>

#include <gtest/gtest.h>

using namespace selector;

/* ***********
 * Ast Node  *
 * **********/
class StubAstNode : public AstNode
{
public:
	StubAstNode() : AstNode() {}

	StubAstNode(const AstNode::Offset_t &startOffset, const AstNode::Offset_t &endOffset)
	    : AstNode(startOffset, endOffset)
	{
	}

	StubAstNode(const AstNode::NodeRange_t &range) : AstNode(range) {}
};

class AstNodeTest : public ::testing::Test
{
protected:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(AstNodeTest, when_using_the_default_constructor_should_set_the_offsets_to_zero)
{
	StubAstNode n;
	EXPECT_EQ(n.GetStartOffset(), 0);
	EXPECT_EQ(n.GetEndOffset(), 0);
}

TEST_F(AstNodeTest, when_constructing_with_offset_should_set_the_offsets)
{
	StubAstNode n(10, 15);
	EXPECT_EQ(n.GetStartOffset(), 10);
	EXPECT_EQ(n.GetEndOffset(), 15);
}

TEST_F(AstNodeTest, when_constructing_with_range_should_set_the_offsets)
{
	StubAstNode n(std::make_pair(10, 15));
	EXPECT_EQ(n.GetStartOffset(), 10);
	EXPECT_EQ(n.GetEndOffset(), 15);
}

TEST_F(AstNodeTest, when_setting_the_range_should_set_the_offsets)
{
	StubAstNode n(std::make_pair(15, 10));
	n.SetNodeRange(std::make_pair(10, 15));
	EXPECT_EQ(n.GetStartOffset(), 10);
	EXPECT_EQ(n.GetEndOffset(), 15);
}

TEST_F(AstNodeTest, when_getting_the_range_should_get_a_correct_range)
{
	auto range = std::make_pair(std::size_t(15), std::size_t(10));
	StubAstNode n(range);
	EXPECT_EQ(n.GetNodeRange(), range);
}

/* ************************
 * Graph Definition Node  *
 * ***********************/
class GraphDefinitionNodeTest : public ::testing::Test
{
protected:
	void SetUp() { m_statements = {std::make_shared<NodeDefinitionNode>(), std::make_shared<NodeLinkNode>()}; }
	void TearDown() {}

	void expectEqualGraphStatements(const std::vector<GraphStatementNodePtr> &actual,
	                                const std::vector<GraphStatementNodePtr> &expected)
	{
		ASSERT_EQ(actual.size(), expected.size());
		for (auto i = 0u; i < actual.size(); ++i)
		{
			EXPECT_EQ(actual[i], expected[i]);
		}
	}

	std::vector<GraphStatementNodePtr> m_statements;
};

TEST_F(GraphDefinitionNodeTest, when_constructing_with_statements_should_set_the_graph_statements)
{
	auto g = std::make_shared<GraphDefinitionNode>(m_statements);
	expectEqualGraphStatements(g->GetGraphStatementNodes(), m_statements);
}

TEST_F(GraphDefinitionNodeTest, when_adding_statements_should_store_the_graph_statements)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	for (const auto &s : m_statements)
	{
		g->AddGraphStatementNode(s);
	}
	expectEqualGraphStatements(g->GetGraphStatementNodes(), m_statements);
}

TEST_F(GraphDefinitionNodeTest, when_setting_statements_should_store_the_graph_statements)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	g->SetGraphStatementNodes(m_statements);
	expectEqualGraphStatements(g->GetGraphStatementNodes(), m_statements);
}

TEST_F(GraphDefinitionNodeTest, when_creating_a_node_definition_node_should_return_and_store_it)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	auto n = g->CreateNodeDefinitionNode("name", "nodeType", std::vector<NamedArgumentPtr>{});
	expectEqualGraphStatements(g->GetGraphStatementNodes(), {n});
}

TEST_F(GraphDefinitionNodeTest, when_creating_a_node_link_node_should_return_and_store_it)
{
	auto g = std::make_shared<GraphDefinitionNode>();
	auto n = g->CreateNodeLinkNode();
	expectEqualGraphStatements(g->GetGraphStatementNodes(), {n});
}

TEST_F(GraphDefinitionNodeTest, when_iterating_over_a_graph_definition_node_should_iterate_over_the_stored_statemets)
{
	auto g = std::make_shared<GraphDefinitionNode>(m_statements);
	std::vector<GraphStatementNodePtr> statementNodes(g->begin(), g->end());  // Does the same thing as iterating
	expectEqualGraphStatements(statementNodes, m_statements);
}

/* *****************
 * Named Argument  *
 * ****************/
class NamedArgumentTest : public ::testing::Test
{
protected:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(NamedArgumentTest, when_constructing_with_name_type_and_value_should_store_them)
{
	auto n = std::make_shared<NamedArgument>("parameter", NamedArgument::ArgumentType::String, "value");
	EXPECT_EQ(n->GetName(), "parameter");
	EXPECT_EQ(n->GetArgumentType(), NamedArgument::ArgumentType::String);
	EXPECT_EQ(n->GetArgumentValue(), "value");
}
