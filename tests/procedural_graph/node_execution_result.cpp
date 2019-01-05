#include <procedural_graph/node_execution_result.h>

#include <gtest/gtest.h>

using namespace selector;

struct ResultValue
{
	ResultValue(int *innerValue) : m_innerValue(innerValue) {}
	~ResultValue() { *m_innerValue = -1; }

	int32_t *m_innerValue;
};

TEST(NodeExecutionResult, when_creating_should_set_result_status)
{
	NodeExecutionResult result(NodeExecutionResult::ResultStatus::Success);
	NodeExecutionResult result2(NodeExecutionResult::ResultStatus::Failure);

	EXPECT_EQ(result.GetResultStatus(), NodeExecutionResult::ResultStatus::Success);
	EXPECT_EQ(result2.GetResultStatus(), NodeExecutionResult::ResultStatus::Failure);
}

TEST(NodeExecutionResult, when_setting_result_should_copy_result)
{
	NodeExecutionResult result(NodeExecutionResult::ResultStatus::Success);

	int32_t *value = new int32_t;
	*value = 10;

	result.SetResult(value, [](void *value) { delete static_cast<int *>(value); });

	EXPECT_EQ(*static_cast<const int32_t *>(result.GetResult()), 10);
}

TEST(NodeExecutionResult, when_setting_result_should_still_be_able_to_change_value_through_variable)
{
	NodeExecutionResult result(NodeExecutionResult::ResultStatus::Success);

	int32_t *value = new int32_t;
	*value = 10;

	result.SetResult(value, [](void *value) { delete static_cast<int *>(value); });

	*value = 123;

	EXPECT_EQ(*static_cast<const int32_t *>(result.GetResult()), 123);
}

TEST(NodeExecutionResult, when_destroying_a_result_should_deallocate_contained_result)
{
	int32_t value = 123;

	{
		NodeExecutionResult result(NodeExecutionResult::ResultStatus::Success);
		result.SetResult(new ResultValue(&value), [](void *value) { delete static_cast<ResultValue *>(value); });
	}

	EXPECT_EQ(value, -1);
}

TEST(NodeExecutionResult, when_setting_a_new_value_should_destroy_previous_value)
{
	int32_t value1 = 123;
	int32_t *value2 = new int32_t;
	*value2 = 321;

	NodeExecutionResult result(NodeExecutionResult::ResultStatus::Success);
	result.SetResult(new ResultValue(&value1), [](void *value) { delete static_cast<ResultValue *>(value); });
	result.SetResult(value2, [](void *value) { delete static_cast<int32_t *>(value); });

	EXPECT_EQ(value1, -1);
}

TEST(NodeExecutionResult, when_destroying_a_result_without_setting_value_should_not_call_delete_function)
{
	try
	{
		NodeExecutionResult result(NodeExecutionResult::ResultStatus::Success);
	}
	catch (std::bad_function_call &e)
	{
		EXPECT_TRUE(false);
	}
}
