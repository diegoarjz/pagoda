#include <parameter/expression.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(ExpressionTest, when_creating_an_expression_should_be_able_to_resolve_variables)
{
	auto expression = Expression::CreateExpression("a + b;");
	auto variables = expression->GetVariables();
	ASSERT_EQ(variables.size(), 2);
	for (auto v : std::vector<std::string>{"a", "b"})
	{
		ASSERT_NE(std::find(std::begin(variables), std::end(variables), v), std::end(variables));
	}
}

TEST(ExpressionTest, when_evaluating_an_expression_should_be_able_to_evaluate_to_floats)
{
	auto expression = Expression::CreateExpression("1 + 2;");
	ASSERT_EQ(expression->GetAsFloat(), 3);
}

TEST(ExpressionTest, when_evaluating_an_expression_should_be_able_to_evaluate_to_string)
{
	auto expression = Expression::CreateExpression("\"abc\" + \"123\";");
	ASSERT_EQ(expression->GetAsString(), "abc123");
}
