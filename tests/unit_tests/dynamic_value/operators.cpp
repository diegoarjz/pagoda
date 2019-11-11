#include <dynamic_value/binding/binary_ops.h>
#include <dynamic_value/binding/has_operators.h>
#include <dynamic_value/binding/unary_ops.h>
#include <dynamic_value/boolean_value.h>
#include <dynamic_value/dynamic_class.h>
#include <dynamic_value/dynamic_instance.h>
#include <dynamic_value/dynamic_value_base.h>
#include <dynamic_value/float_value.h>
#include <dynamic_value/function.h>
#include <dynamic_value/integer_value.h>
#include <dynamic_value/null_object_value.h>
#include <dynamic_value/string_value.h>
#include <dynamic_value/type_info.h>
#include <dynamic_value/vector3.h>
#include <dynamic_value/expression.h>

#include <gtest/gtest.h>

#include <iostream>
#include "../test_utils.h"

using namespace selector;

TEST(UnaryOperators, test_valid_negate_operator)
{
	Boolean b(true);
	unary_ops_dispatcher<negate> dispatcher;
	BooleanPtr result = std::dynamic_pointer_cast<Boolean>(apply_visitor(dispatcher, b));
	ASSERT_NE(result, nullptr);
	EXPECT_FALSE(static_cast<bool>(*result));
}

TEST(UnaryOperators, test_invalid_negate_operator)
{
	FloatValue v(123.0f);
	unary_ops_dispatcher<negate> dispatcher;
	try
	{
		apply_visitor(dispatcher, v);
	}
	catch (UndefinedNegateOperator<FloatValue> &)
	{
		return;
	}
	FAIL() << "Should have thrown";
}

TEST(UnaryOperators, test_valid_minus_operator)
{
	FloatValue v(123.0f);
	unary_ops_dispatcher<minus> dispatcher;
	FloatValuePtr result = std::dynamic_pointer_cast<FloatValue>(apply_visitor(dispatcher, v));
	ASSERT_NE(result, nullptr);
	EXPECT_EQ(-123.0f, static_cast<float>(*result));
}

TEST(UnaryOperators, test_invalid_minus_operator)
{
	Boolean v(true);
	unary_ops_dispatcher<minus> dispatcher;
	try
	{
		apply_visitor(dispatcher, v);
	}
	catch (UndefinedMinusOperator<Boolean> &)
	{
		return;
	}
	FAIL() << "Should have thrown";
}

TEST(BinaryOperators, test_valid_add_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(1.0f);

	binary_op_dispatcher<add> dispatcher(f1, f2);
	FloatValuePtr result = std::dynamic_pointer_cast<FloatValue>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_EQ(124.0f, static_cast<float>(*result));
}

TEST(BinaryOperators, test_invalid_add_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto b = std::make_shared<Boolean>(true);

	binary_op_dispatcher<add> dispatcher(f1, b);

	try
	{
		apply_visitor(dispatcher, *f1);
	}
	catch (UndefinedBinaryOperator<FloatValue, Boolean> &)
	{
		return;
	}
	FAIL() << "Should have thrown";
}

TEST(BinaryOperators, test_valid_sub_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(1.0f);

	binary_op_dispatcher<sub> dispatcher(f1, f2);
	FloatValuePtr result = std::dynamic_pointer_cast<FloatValue>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_EQ(122.0f, static_cast<float>(*result));
}

TEST(BinaryOperators, test_valid_mul_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(2.0f);

	binary_op_dispatcher<mul> dispatcher(f1, f2);
	FloatValuePtr result = std::dynamic_pointer_cast<FloatValue>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_EQ(246.0f, static_cast<float>(*result));
}

TEST(BinaryOperators, test_valid_div_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(2.0f);

	binary_op_dispatcher<selector::div> dispatcher(f1, f2);
	FloatValuePtr result = std::dynamic_pointer_cast<FloatValue>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_EQ(123.0f / 2.0f, static_cast<float>(*result));
}

TEST(BinaryOperators, test_valid_eq_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(123.0f);

	binary_op_dispatcher<selector::eq> dispatcher(f1, f2);
	BooleanPtr result = std::dynamic_pointer_cast<Boolean>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_TRUE(static_cast<bool>(*result));
}

TEST(BinaryOperators, test_valid_neq_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(23.0f);

	binary_op_dispatcher<selector::neq> dispatcher(f1, f2);
	BooleanPtr result = std::dynamic_pointer_cast<Boolean>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_TRUE(static_cast<bool>(*result));
}

TEST(BinaryOperators, test_valid_gt_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(23.0f);

	binary_op_dispatcher<selector::gt> dispatcher(f1, f2);
	BooleanPtr result = std::dynamic_pointer_cast<Boolean>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_TRUE(static_cast<bool>(*result));
}

TEST(BinaryOperators, test_valid_gte_operator)
{
	auto f1 = std::make_shared<FloatValue>(123.0f);
	auto f2 = std::make_shared<FloatValue>(23.0f);

	binary_op_dispatcher<selector::gte> dispatcher(f1, f2);
	BooleanPtr result = std::dynamic_pointer_cast<Boolean>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_TRUE(static_cast<bool>(*result));
}

TEST(BinaryOperators, test_valid_lt_operator)
{
	auto f2 = std::make_shared<FloatValue>(123.0f);
	auto f1 = std::make_shared<FloatValue>(23.0f);

	binary_op_dispatcher<selector::lt> dispatcher(f1, f2);
	BooleanPtr result = std::dynamic_pointer_cast<Boolean>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_TRUE(static_cast<bool>(*result));
}

TEST(BinaryOperators, test_valid_lte_operator)
{
	auto f2 = std::make_shared<FloatValue>(123.0f);
	auto f1 = std::make_shared<FloatValue>(23.0f);

	binary_op_dispatcher<selector::lte> dispatcher(f1, f2);
	BooleanPtr result = std::dynamic_pointer_cast<Boolean>(apply_visitor(dispatcher, *f1));
	ASSERT_NE(result, nullptr);
	EXPECT_TRUE(static_cast<bool>(*result));
}
