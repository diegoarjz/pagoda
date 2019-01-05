#include <procedural_graph/graph_execution_context.h>
#include <procedural_graph/node.h>
#include <procedural_graph/node_execution_result.h>

#include "../parameter/mock_objects.h"
#include "mock_objects.h"

#include <gtest/gtest.h>

using namespace selector;

class NodeTest : public ::testing::TestWithParam<NodeType>
{
protected:
	void SetUp() { node = std::make_shared<Node>(GetParam()); }

	void TearDown() {}

	std::shared_ptr<Node> node;
};

TEST_P(NodeTest, when_creating_should_set_node_type) { EXPECT_EQ(this->node->GetNodeType(), GetParam()); }

TEST_P(NodeTest, when_setting_node_id_should_be_able_to_get_node_id)
{
	this->node->SetId(123);
	EXPECT_EQ(this->node->GetId(), 123);
}

TEST_P(NodeTest, when_getting_node_id_after_construction_should_return_0) { EXPECT_EQ(this->node->GetId(), 0); }

TEST_P(NodeTest, when_setting_node_name_should_be_able_to_get_node_name)
{
	this->node->SetName("nodeName");
	EXPECT_EQ(this->node->GetName(), "nodeName");
}

TEST_P(NodeTest, when_getting_node_name_after_construction_should_return_empty_name)
{
	EXPECT_EQ(this->node->GetName(), "");
}

TEST_P(NodeTest, when_getting_node_execution_result_before_being_executed_should_return_nullptr)
{
	EXPECT_EQ(this->node->GetResult(), nullptr);
}

TEST_P(NodeTest, when_setting_a_node_execution_should_be_able_to_get_the_same_execution)
{
	auto execution = std::make_shared<MockNodeExecution>();
	this->node->SetNodeExecution(execution);
	EXPECT_EQ(this->node->GetNodeExecution(), execution);
}

TEST_P(NodeTest, when_getting_the_node_execution_before_setting_should_return_nullptr)
{
	EXPECT_EQ(this->node->GetNodeExecution(), nullptr);
}

TEST_P(NodeTest, when_setting_parameter_context_should_be_able_to_get_the_same_context)
{
	auto context = std::make_shared<ContextMock>(ParameterIdentifier::CreateIdentifier("a").second);
	this->node->SetParameterContext(context);
	EXPECT_EQ(this->node->GetParameterContext(), context);
}

TEST_P(NodeTest, when_getting_parameter_context_before_setting_should_return_nullptr)
{
	EXPECT_EQ(this->node->GetParameterContext(), nullptr);
}

TEST_P(NodeTest, when_executing_node_should_call_node_execution)
{
	auto execution = std::make_shared<MockNodeExecution>();
	auto executionContext = std::make_shared<GraphExecutionContext>(nullptr, nullptr, nullptr);
	auto executionResult = std::make_shared<NodeExecutionResult>(NodeExecutionResult::ResultStatus::Success);

	// clang-format off
    EXPECT_CALL(*execution, Execute(this->node, executionContext))
        .Times(1)
        .WillOnce(::testing::Return(executionResult));
	// clang-format on

	this->node->SetNodeExecution(execution);
	this->node->Execute(executionContext);

	// passing this->node to EXPECT_CALL is causing a cyclic dependency
	// between the node and the execution. Mock objects are not being released.
	// Setting the node execution to nullptr fixes this
	this->node->SetNodeExecution(nullptr);
}

TEST_P(NodeTest, when_executing_should_set_node_result)
{
	auto execution = std::make_shared<MockNodeExecution>();
	auto executionContext = std::make_shared<GraphExecutionContext>(nullptr, nullptr, nullptr);
	auto executionResult = std::make_shared<NodeExecutionResult>(NodeExecutionResult::ResultStatus::Success);

	// clang-format off
    EXPECT_CALL(*execution, Execute(this->node, executionContext))
        .Times(1)
        .WillOnce(::testing::Return(executionResult));
	// clang-format on

	this->node->SetNodeExecution(execution);
	this->node->Execute(executionContext);

	EXPECT_EQ(this->node->GetResult(), executionResult);

	this->node->SetNodeExecution(nullptr);
}

TEST_P(NodeTest, when_executing_before_setting_node_execution_should_not_execute)
{
	auto executionContext = std::make_shared<GraphExecutionContext>(nullptr, nullptr, nullptr);
	this->node->Execute(executionContext);

	EXPECT_EQ(this->node->GetResult(), nullptr);
}

TEST_P(NodeTest, when_executing_with_a_null_execution_context_should_not_execute)
{
	auto execution = std::make_shared<MockNodeExecution>();

	// clang-format off
    EXPECT_CALL(*execution, Execute(this->node, testing::_))
        .Times(0);
	// clang-format on

	this->node->SetNodeExecution(execution);
	this->node->Execute(nullptr);
	EXPECT_EQ(this->node->GetResult(), nullptr);

	this->node->SetNodeExecution(nullptr);
}

INSTANTIATE_TEST_CASE_P(BuiltInNodeTypes, NodeTest,
                        ::testing::Values(NodeType::InputInterface, NodeType::Operation, NodeType::OutputInterface));
