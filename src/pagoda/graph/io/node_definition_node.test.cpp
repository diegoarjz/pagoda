#include <pagoda/graph/io/graph_definition_node.h>
#include <pagoda/graph/io/named_argument.h>
#include <pagoda/graph/io/node_definition_node.h>
#include <pagoda/graph/io/node_link_definition.h>
#include <pagoda/graph/io/node_link_node.h>
#include <memory>

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::graph;
using namespace pagoda::graph::io;

/* **********************
 * Node Definition Node *
 * *********************/
class NodeDefinitionNodeTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
		constructionArgs.push_back(std::make_shared<NamedArgument>("arg", NamedArgument::ArgumentType::String, "value"));
		executionArgs.push_back(std::make_shared<NamedArgument>("execArg", NamedArgument::ArgumentType::String, "value"));
	}

	void TearDown() {}

	template<typename Container>
	void expectEqualArgs(const Container &t1, const Container &t2)
	{
		ASSERT_EQ(t1.size(), t2.size());
		auto t1Iter = std::begin(t1);
		auto t2Iter = std::begin(t2);
		for (auto i = 0u; i < t1.size(); ++i, ++t1Iter, ++t2Iter) {
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
	auto n = std::make_shared<NodeDefinitionNode>(std::make_pair(std::size_t(0), std::size_t(1)), "nodeName", "Operation",
	                                              constructionArgs);
	EXPECT_EQ(n->GetNodeName(), "nodeName");
	EXPECT_EQ(n->GetNodeType(), "Operation");
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
	EXPECT_EQ(n->GetExecutionArguments().size(), 0);
}

TEST_F(NodeDefinitionNodeTest, when_setting_the_construction_arguments_should_store_them)
{
	auto n = std::make_shared<NodeDefinitionNode>(std::make_pair(std::size_t(0), std::size_t(1)), "nodeName", "Operation",
	                                              NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->SetConstructionArguments(constructionArgs);
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_setting_the_execution_arguments_should_store_them)
{
	auto n = std::make_shared<NodeDefinitionNode>(0, 0, "n", "o", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->SetExecutionArguments(executionArgs);
	expectEqualArgs(n->GetExecutionArguments(), executionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_adding_a_construction_arg_should_store_it)
{
	auto n = std::make_shared<NodeDefinitionNode>(0, 0, "n", "o", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->AddConstructionArgument(constructionArgs[0]);
	expectEqualArgs(n->GetConstructionArguments(), constructionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_adding_an_execution_arg_should_store_it)
{
	auto n = std::make_shared<NodeDefinitionNode>(0, 0, "n", "0", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	n->AddExecutionArgument(executionArgs[0]);
	expectEqualArgs(n->GetExecutionArguments(), executionArgs);
}

TEST_F(NodeDefinitionNodeTest, when_creating_a_construction_arg_should_store_it)
{
	auto n = std::make_shared<NodeDefinitionNode>(0, 0, "n", "0", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	auto a = n->CreateConstructionArgument("a", NamedArgument::ArgumentType::String, "abc");
	expectEqualArgs(n->GetConstructionArguments(), {a});
}

TEST_F(NodeDefinitionNodeTest, when_creating_an_execution_arg_should_store_it)
{
	auto n = std::make_shared<NodeDefinitionNode>(0, 0, "n", "0", NodeDefinitionNode::ConstructionArgumentContainer_t{});
	auto a = n->CreateExecutionArgument("a", NamedArgument::ArgumentType::String, "abc");
	expectEqualArgs(n->GetExecutionArguments(), {a});
}

