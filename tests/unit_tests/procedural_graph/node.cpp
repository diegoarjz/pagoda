#include <procedural_graph/graph_execution_context.h>
#include <procedural_graph/operation_node.h>

#include "../parameter/mock_objects.h"
#include "mock_objects.h"

#include <gtest/gtest.h>

using namespace selector;

class NodeTest : public ::testing::Test
{
protected:
	void SetUp() { node = std::make_shared<OperationNode>(); }

	void TearDown() {}

	std::shared_ptr<Node> node;
};

TEST_F(NodeTest, when_setting_node_id_should_be_able_to_get_node_id)
{
	this->node->SetId(123);
	EXPECT_EQ(this->node->GetId(), 123);
}

TEST_F(NodeTest, when_getting_node_id_after_construction_should_return_0) { EXPECT_EQ(this->node->GetId(), 0); }

TEST_F(NodeTest, when_setting_node_name_should_be_able_to_get_node_name)
{
	this->node->SetName("nodeName");
	EXPECT_EQ(this->node->GetName(), "nodeName");
}

TEST_F(NodeTest, when_getting_node_name_after_construction_should_return_empty_name)
{
	EXPECT_EQ(this->node->GetName(), "");
}

TEST_F(NodeTest, when_setting_parameter_context_should_be_able_to_get_the_same_context)
{
	auto context = std::make_shared<ContextMock>("a");
	this->node->SetParameterContext(context);
	EXPECT_EQ(this->node->GetParameterContext(), context);
}

TEST_F(NodeTest, when_getting_parameter_context_before_setting_should_return_nullptr)
{
	EXPECT_EQ(this->node->GetParameterContext(), nullptr);
}

TEST_F(NodeTest, when_setting_expression_variables_should_set_the_variables_in_expressions)
{
	auto context = std::make_shared<Context>("a");
	auto expr = Expression::CreateExpression("par * 2;");
	context->SetParameter("par1", expr);
	context->SetParameter("par", 3.0f);
	node->SetParameterContext(context);
	node->SetExpressionVariables();
	ASSERT_EQ(expr->GetAsFloat(), 6.0f);
}
