#include <parameter/expression.h>

#include <gtest/gtest.h>

using namespace selector;

class ExpressionTest : public ::testing::Test
{
protected:
	void SetUp() {}

	void TearDown() {}
};

TEST_F(ExpressionTest, when_creating_an_expression_should_be_able_to_resolve_variables)
{
	auto expression = Expression::CreateExpression("a + b2;");
	auto variables = expression->GetVariables();
	ASSERT_EQ(variables.size(), 2);
	for (auto v : std::vector<std::string>{"a", "b2"})
	{
		ASSERT_NE(std::find(std::begin(variables), std::end(variables), v), std::end(variables));
	}
}

TEST_F(ExpressionTest, when_resolving_variables_should_be_able_to_get_compound_variables)
{
	auto expression = Expression::CreateExpression("a.b.c + d.e;");
	auto variables = expression->GetVariables();
	ASSERT_EQ(variables.size(), 2);
}

TEST_F(ExpressionTest, when_evaluating_an_expression_should_be_able_to_evaluate_to_floats)
{
	auto expression = Expression::CreateExpression("1.0 + 2.0;");
	ASSERT_EQ(expression->GetAsFloat(), 3);
	ASSERT_EQ(std::get<float>(expression->GetAsParameter()), 3);
}

TEST_F(ExpressionTest, when_evaluating_an_expression_should_be_able_to_evaluate_to_string)
{
	auto expression = Expression::CreateExpression("\"abc\" + \"123\";");
	ASSERT_EQ(expression->GetAsString(), "abc123");
	ASSERT_EQ(std::get<std::string>(expression->GetAsParameter()), "abc123");
}

TEST_F(ExpressionTest, when_evaluating_an_expression_with_variables_should_be_able_to_resolve_their_value)
{
	auto expression = Expression::CreateExpression("a + b;");
	expression->SetVariableValue("a", "123");
	expression->SetVariableValue("b", "abc");
	ASSERT_EQ(expression->GetAsString(), "123abc");
}

TEST_F(ExpressionTest, when_a_variable_is_an_expression_should_calculate_its_value_first)
{
	auto expression = Expression::CreateExpression("a + b;");
	auto expression2 = Expression::CreateExpression("\"123\" + \"abc\";");
	expression->SetVariableValue("a", "abc");
	expression->SetVariableValue("b", expression2);
	ASSERT_EQ(expression->GetAsString(), "abc123abc");
}

TEST_F(ExpressionTest, when_setting_an_expression_as_avariable_should_add_to_its_dependent_expressions)
{
	auto expression = Expression::CreateExpression("a * b;");
	auto expression2 = Expression::CreateExpression("\"123\" + \"abc\";");
	expression->SetVariableValue("b", expression2);
	ASSERT_EQ(expression2->GetDependentExpressions().size(), 1);
	ASSERT_EQ(expression2->GetDependentExpressions().front().lock(), expression);
}

TEST_F(ExpressionTest, when_evaluating_an_expression_should_set_it_as_clean)
{
	auto expression = Expression::CreateExpression("a + b;");
	auto expression2 = Expression::CreateExpression("\"123\" + \"abc\";");
	expression->SetVariableValue("a", "abc");
	expression->SetVariableValue("b", expression2);
	expression->GetAsString();  // Evaluate

	ASSERT_FALSE(expression->IsDirty());
	ASSERT_FALSE(expression2->IsDirty());
}

TEST_F(ExpressionTest, when_setting_an_expression_as_dirty_should_propagate_to_dependent_expressions)
{
	auto expression = Expression::CreateExpression("a + b;");
	auto expression2 = Expression::CreateExpression("\"123\" + \"abc\";");
	expression->SetVariableValue("a", "abc");
	expression->SetVariableValue("b", expression2);
	expression->GetAsString();  // Evaluate, setting everything as not dirty
	expression2->SetDirty();

	ASSERT_TRUE(expression->IsDirty());
	ASSERT_TRUE(expression2->IsDirty());
}
