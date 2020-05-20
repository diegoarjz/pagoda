#include <pagoda/dynamic/binding/has_operators.h>
#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/dynamic_value_base.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/integer_value.h>
#include <pagoda/dynamic/null_object_value.h>
#include <pagoda/dynamic/string_value.h>
#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/vector3.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;
using namespace pagoda::dynamic;

template<class LHS, class RHS>
bool assertHasAddOperator()
{
	return (has_add_operator<LHS, RHS>::value);
}

template<class LHS, class RHS>
bool assertHasSubOperator()
{
	return has_sub_operator<LHS, RHS>::value;
}

template<class LHS, class RHS>
bool assertHasMulOperator()
{
	return has_mul_operator<LHS, RHS>::value;
}

template<class LHS, class RHS>
bool assertHasDivOperator()
{
	return has_div_operator<LHS, RHS>::value;
}

TEST(DynamicValueOperators, test_valid_add_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasAddOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasAddOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasAddOperator<Integer, Integer>()));
	EXPECT_TRUE((assertHasAddOperator<Integer, FloatValue>()));

	// String on LHS
	EXPECT_TRUE((assertHasAddOperator<String, String>()));

	// Vector3
	EXPECT_TRUE((assertHasAddOperator<Vector3, Vector3>()));
}

TEST(DynamicValueOperators, test_valid_sub_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasSubOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasSubOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasSubOperator<Integer, Integer>()));
	EXPECT_TRUE((assertHasSubOperator<Integer, FloatValue>()));

	// Vector3
	EXPECT_TRUE((assertHasSubOperator<Vector3, Vector3>()));
}

TEST(DynamicValueOperators, test_valid_mul_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasMulOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasMulOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasMulOperator<Integer, Integer>()));
	EXPECT_TRUE((assertHasMulOperator<Integer, FloatValue>()));

	// Vector3
	EXPECT_TRUE((assertHasMulOperator<Vector3, Integer>()));
	EXPECT_TRUE((assertHasMulOperator<Vector3, FloatValue>()));
}

TEST(DynamicValueOperators, test_valid_div_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasDivOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasDivOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasDivOperator<Integer, Integer>()));
	EXPECT_TRUE((assertHasDivOperator<Integer, FloatValue>()));

	// Vector3
	EXPECT_TRUE((assertHasDivOperator<Vector3, Integer>()));
	EXPECT_TRUE((assertHasDivOperator<Vector3, FloatValue>()));
}

/*
 * Logical operators
 */
template<class LHS, class RHS>
bool assertHasEqOperator()
{
	return (has_eq_operator<LHS, RHS>::value);
}

template<class LHS, class RHS>
bool assertHasNeqOperator()
{
	return (has_neq_operator<LHS, RHS>::value);
}

template<class LHS, class RHS>
bool assertHasGtOperator()
{
	return (has_gt_operator<LHS, RHS>::value);
}

template<class LHS, class RHS>
bool assertHasGteOperator()
{
	return (has_gte_operator<LHS, RHS>::value);
}

template<class LHS, class RHS>
bool assertHasLtOperator()
{
	return (has_lt_operator<LHS, RHS>::value);
}

template<class LHS, class RHS>
bool assertHasLteOperator()
{
	return (has_lte_operator<LHS, RHS>::value);
}

template<class LHS>
bool assertHasMinusOperator()
{
	return (has_minus_operator<LHS>::value);
}

template<class LHS>
bool assertHasNegateOperator()
{
	return (has_negate_operator<LHS>::value);
}

TEST(DynamicValueOperators, test_valid_eq_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasEqOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasEqOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasEqOperator<Integer, FloatValue>()));
	EXPECT_TRUE((assertHasEqOperator<Integer, Integer>()));

	// String on LHS
	EXPECT_TRUE((assertHasEqOperator<String, String>()));

	// Vector3 on LHS
	EXPECT_TRUE((assertHasEqOperator<Vector3, Vector3>()));
}

TEST(DynamicValueOperators, test_valid_neq_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasNeqOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasNeqOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasNeqOperator<Integer, FloatValue>()));
	EXPECT_TRUE((assertHasNeqOperator<Integer, Integer>()));

	// String on LHS
	EXPECT_TRUE((assertHasNeqOperator<String, String>()));

	// Vector3 on LHS
	EXPECT_TRUE((assertHasNeqOperator<Vector3, Vector3>()));
}

TEST(DynamicValueOperators, test_valid_gt_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasGtOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasGtOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasGtOperator<Integer, FloatValue>()));
	EXPECT_TRUE((assertHasGtOperator<Integer, Integer>()));
}

TEST(DynamicValueOperators, test_valid_geq_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasGteOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasGteOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasGteOperator<Integer, FloatValue>()));
	EXPECT_TRUE((assertHasGteOperator<Integer, Integer>()));
}

TEST(DynamicValueOperators, test_valid_lt_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasLtOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasLtOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasLtOperator<Integer, FloatValue>()));
	EXPECT_TRUE((assertHasLtOperator<Integer, Integer>()));
}

TEST(DynamicValueOperators, test_valid_leq_operators)
{
	// Float on LHS
	EXPECT_TRUE((assertHasLteOperator<FloatValue, FloatValue>()));
	EXPECT_TRUE((assertHasLteOperator<FloatValue, Integer>()));

	// Integer on LHS
	EXPECT_TRUE((assertHasLteOperator<Integer, FloatValue>()));
	EXPECT_TRUE((assertHasLteOperator<Integer, Integer>()));
}

TEST(DynamicValueOperators, test_valid_minus_operators)
{
	// Float
	EXPECT_TRUE((assertHasMinusOperator<FloatValue>()));

	// Integer
	EXPECT_TRUE((assertHasMinusOperator<Integer>()));
}

TEST(DynamicValueOperators, test_valid_negate_operators)
{
	// Boolean
	EXPECT_TRUE((assertHasNegateOperator<Boolean>()));
}

TEST(DynamicValueOperators, test_invalid_operators) { EXPECT_FALSE((assertHasSubOperator<FloatValue, Boolean>())); }
