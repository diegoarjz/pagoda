#include <procedural_graph/reader/graph_definition_node.h>
#include <procedural_graph/reader/named_argument.h>
#include <procedural_graph/reader/node_definition_node.h>
#include <procedural_graph/reader/node_link_node.h>

#include <gtest/gtest.h>

using namespace pagoda;

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

	void AcceptVisitor(AstNodeVisitor *visitor) override {}
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

TEST_F(NamedArgumentTest, when_setting_name_type_and_value_should_store_them)
{
	auto n = std::make_shared<NamedArgument>("parameter", NamedArgument::ArgumentType::String, "value");
	n->SetName("itsName");
	n->SetArgumentType(NamedArgument::ArgumentType::Float);
	n->SetArgumentValue("itsValue");
	EXPECT_EQ(n->GetName(), "itsName");
	EXPECT_EQ(n->GetArgumentType(), NamedArgument::ArgumentType::Float);
	EXPECT_EQ(n->GetArgumentValue(), "itsValue");
}

/* **********************
 * Node Definition Node *
 * *********************/
class NodeDefinitionNodeTest : public ::testing::Test
{
protected:
	void SetUp()
	{
		constructionArgs.push_back(
		    std::make_shared<NamedArgument>("arg", NamedArgument::ArgumentType::String, "value"));
		executionArgs.push_back(
		    std::make_shared<NamedArgument>("execArg", NamedArgument::ArgumentType::String, "value"));
	}

	void TearDown() {}

	template<typename Container>
	void expectEqualArgs(const Container &t1, const Container &t2)
	{
		ASSERT_EQ(t1.size(), t2.size());
		auto t1Iter = std::begin(t1);
		auto t2Iter = std::begin(t2);
		for (auto i = 0u; i < t1.size(); ++i, ++t1Iter, ++t2Iter)
		{
			EXPECT_EQ(*t1Iter, *t2Iter);
		}
	}

	NodeDefinitionNode::ConstructionArgumentContainer_t constructionArgs;
	NodeDefinitionNode::ExecutionArgumentContainer_t executionArgs;
};

TEST_F(NodeDefinitionNodeTest, when_constructing_with_name_node_type_and_construction_arguments_should_store_them)
{
	auto n = std::make_shared<NodeDefinitionNode>("nodeName", "Operation", constructionArgs);
	EXPECT_EQ(n->GetNodeName(), "nodeName");
	EXPECT_EQ(n->GetNodeType(), "Operation");
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
	EXPECT_EQ(n->GetExecutionArguments().size(), 0);
}

TEST_F(NodeDefinitionNodeTest,
       when_constructing_with_offsets_name_node_type_and_construction_arguments_should_store_them)
{
	auto n = std::make_shared<NodeDefinitionNode>(0, 0, "nodeName", "Operation", constructionArgs);
	EXPECT_EQ(n->GetNodeName(), "nodeName");
	EXPECT_EQ(n->GetNodeType(), "Operation");
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
	EXPECT_EQ(n->GetExecutionArguments().size(), 0);
}

TEST_F(NodeDefinitionNodeTest, when_constructing_with_range_name_node_type_and_construction_arguments_should_store_them)
{
	auto n = std::make_shared<NodeDefinitionNode>(std::make_pair(std::size_t(0), std::size_t(1)), "nodeName",
	                                              "Operation", constructionArgs);
	EXPECT_EQ(n->GetNodeName(), "nodeName");
	EXPECT_EQ(n->GetNodeType(), "Operation");
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
	EXPECT_EQ(n->GetExecutionArguments().size(), 0);
}

TEST_F(NodeDefinitionNodeTest, when_setting_the_construction_arguments_should_store_them)
{
	auto n = std::make_shared<NodeDefinitionNode>(std::make_pair(std::size_t(0), std::size_t(1)), "nodeName",
	                                              "Operation", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->SetConstructionArguments(constructionArgs);
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_setting_the_execution_arguments_should_store_them)
{
	auto n =
	    std::make_shared<NodeDefinitionNode>(0, 0, "n", "o", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->SetExecutionArguments(executionArgs);
	expectEqualArgs(n->GetExecutionArguments(), executionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_adding_a_construction_arg_should_store_it)
{
	auto n =
	    std::make_shared<NodeDefinitionNode>(0, 0, "n", "o", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->AddConstructionArgument(constructionArgs[0]);
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_adding_an_execution_arg_should_store_it)
{
	auto n =
	    std::make_shared<NodeDefinitionNode>(0, 0, "n", "0", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->AddExecutionArgument(executionArgs[0]);
	expectEqualArgs(n->GetExecutionArguments(), executionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_creating_a_construction_arg_should_store_it)
{
	auto n =
	    std::make_shared<NodeDefinitionNode>(0, 0, "n", "0", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	auto a = n->CreateConstructionArgument("a", NamedArgument::ArgumentType::String, "abc");
	expectEqualArgs(n->GetConstructionArguments(), {a});
}

TEST_F(NodeDefinitionNodeTest, when_creating_an_execution_arg_should_store_it)
{
	auto n =
	    std::make_shared<NodeDefinitionNode>(0, 0, "n", "0", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	auto a = n->CreateExecutionArgument("a", NamedArgument::ArgumentType::String, "abc");
	expectEqualArgs(n->GetExecutionArguments(), {a});
}

/* ****************
 * Node Link Node *
 * ***************/

class NodeLinkNodeTest : public ::testing::Test
{
protected:
	void SetUp() { m_linkedNodes = {"n1", "n2", "n3"}; }

	void TearDown() {}

	void expectEqualLinkedNodes(const NodeLinkNode::NodeLinkContainer_t &t1,
	                            const NodeLinkNode::NodeLinkContainer_t &t2)
	{
		ASSERT_EQ(t1.size(), t2.size());

		auto t1Iter = std::begin(t1);
		auto t2Iter = std::begin(t2);
		for (auto i = 0u; i < t1.size(); ++i, ++t1Iter, ++t2Iter)
		{
			EXPECT_EQ(*t1Iter, *t2Iter);
		}
	}

	NodeLinkNode::NodeLinkContainer_t m_linkedNodes;
};

TEST_F(NodeLinkNodeTest, when_constructing_with_offsets_and_linked_nodes_should_store_them)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, m_linkedNodes);
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_constructing_with_range_and_linked_nodes_should_store_them)
{
	auto n = std::make_shared<NodeLinkNode>(std::make_pair(std::size_t(0), std::size_t(1)), m_linkedNodes);
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_setting_linked_nodes_should_store_them)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, NodeLinkNode::NodeLinkContainer_t{});
	n->SetLinkedNodes(m_linkedNodes);
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_adding_a_linked_node_should_store_it)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, NodeLinkNode::NodeLinkContainer_t{});
	for (auto l : m_linkedNodes)
	{
		n->AddLinkedNode(l);
	}
	expectEqualLinkedNodes(n->GetLinkedNodes(), m_linkedNodes);
}

TEST_F(NodeLinkNodeTest, when_iterating_over_should_access_the_linked_nodes_in_order)
{
	auto n = std::make_shared<NodeLinkNode>(0, 1, m_linkedNodes);
	NodeLinkNode::NodeLinkContainer_t c(n->begin(), n->end());  // does the same as iterating
	expectEqualLinkedNodes(c, m_linkedNodes);
}

