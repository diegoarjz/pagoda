#include <dynamic_value/expression.h>
#include <dynamic_value/get_value_as.h>

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
	auto expression = Expression::CreateExpression("a + b;");
	auto variables = expression->GetVariables();
	ASSERT_EQ(variables.size(), 2);
	for (auto v : std::vector<std::string>{"a", "b"})
	{
		ASSERT_NE(std::find(std::begin(variables), std::end(variables), v), std::end(variables));
	}
}

TEST_F(ExpressionTest, when_evaluating_an_expression_should_be_able_to_evaluate_to_floats)
{
	auto expression = Expression::CreateExpression("1.0 + 2.0;");
	float f = get_value_as<float>(*expression);
	ASSERT_EQ(f, 3.0f);
}

TEST_F(ExpressionTest, when_evaluating_an_expression_should_be_able_to_evaluate_to_string)
{
	auto expression = Expression::CreateExpression("\"abc\" + \"123\";");
    std::string s = get_value_as<std::string>(*expression);
    ASSERT_EQ(s, "abc123");
}

TEST_F(ExpressionTest, when_evaluating_an_expression_should_be_able_to_evaluate_to_integers)
{
	auto expression = Expression::CreateExpression("1 + 2;");
    int i = get_value_as<int>(*expression);
	ASSERT_EQ(i, 3);
}

TEST_F(ExpressionTest, when_evaluating_an_expression_with_variables_should_be_able_to_resolve_their_value)
{
	auto expression = Expression::CreateExpression("a + b;");
	expression->SetVariableValue("a", std::make_shared<String>("123"));
	expression->SetVariableValue("b", std::make_shared<String>("abc"));
	ASSERT_EQ(get_value_as<std::string>(*expression), "123abc");
}

TEST_F(ExpressionTest, when_a_variable_is_an_expression_should_calculate_its_value_first)
{
	auto expression = Expression::CreateExpression("a + b;");
	auto expression2 = Expression::CreateExpression("\"123\" + \"abc\";");
	expression->SetVariableValue("a", std::make_shared<String>("abc"));
	expression->SetVariableValue("b", expression2);
	ASSERT_EQ(get_value_as<std::string>(*expression), "abc123abc");
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
	expression->SetVariableValue("a", std::make_shared<String>("abc"));
	expression->SetVariableValue("b", expression2);
	expression->Evaluate();

	ASSERT_FALSE(expression->IsDirty());
	ASSERT_FALSE(expression2->IsDirty());
}

TEST_F(ExpressionTest, when_setting_an_expression_as_dirty_should_propagate_to_dependent_expressions)
{
	auto expression = Expression::CreateExpression("a + b;");
	auto expression2 = Expression::CreateExpression("\"123\" + \"abc\";");
	expression->SetVariableValue("a", std::make_shared<String>("abc"));
	expression->SetVariableValue("b", expression2);
	expression->Evaluate();
	expression2->SetDirty();

	ASSERT_TRUE(expression->IsDirty());
	ASSERT_TRUE(expression2->IsDirty());
}
