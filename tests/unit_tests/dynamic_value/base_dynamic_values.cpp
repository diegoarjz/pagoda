#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/dynamic_value_base.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/integer_value.h>
#include <pagoda/dynamic/null_object_value.h>
#include <pagoda/dynamic/set_value_from.h>
#include <pagoda/dynamic/string_value.h>
#include <pagoda/dynamic/type_info.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;
using namespace pagoda::dynamic;

TEST(FloatValue, test_constructions)
{
	EXPECT_EQ(0, static_cast<float>(FloatValue()));
	EXPECT_EQ(123.123f, static_cast<float>(FloatValue(123.123f)));
	EXPECT_EQ(123.0, static_cast<float>(FloatValue(123)));
	EXPECT_EQ(123.0, static_cast<float>(FloatValue(Integer(123))));
	EXPECT_EQ(123.0, static_cast<float>(FloatValue(std::make_shared<Integer>(123))));
}

TEST(FloatValue, test_arithmetics)
{
	FloatValue a(123);
	FloatValue b(3.5f);
	Integer c(3);

	EXPECT_EQ(static_cast<float>(-a), -123);

	EXPECT_EQ(static_cast<float>(a + b), 123 + 3.5f);
	EXPECT_EQ(static_cast<float>(a - b), 123 - 3.5f);
	EXPECT_EQ(static_cast<float>(a * b), 123 * 3.5f);
	EXPECT_EQ(static_cast<float>(a / b), 123 / 3.5f);

	EXPECT_EQ(static_cast<float>(a + c), 126.0f);
	EXPECT_EQ(static_cast<float>(a - c), 120.0f);
	EXPECT_EQ(static_cast<float>(a * c), 123 * 3);
	EXPECT_EQ(static_cast<float>(a / c), 123 / 3);
}

TEST(FloatValue, test_comparisons)
{
	FloatValue a(123);
	FloatValue b(3.5f);
	Integer c(3);

	EXPECT_EQ(true, a == a);
	EXPECT_EQ(false, a == b);
	EXPECT_EQ(true, a != b);
	EXPECT_EQ(false, a != a);
	EXPECT_EQ(false, a > a);
	EXPECT_EQ(true, a > b);
	EXPECT_EQ(false, a < a);
	EXPECT_EQ(false, a < b);
	EXPECT_EQ(true, a <= a);
	EXPECT_EQ(false, a <= b);
	EXPECT_EQ(true, a >= a);
	EXPECT_EQ(true, b <= a);

	EXPECT_EQ(true, a == Integer(123));
	EXPECT_EQ(true, a > c);
	EXPECT_EQ(true, a != c);
}

TEST(FloatValue, test_can_cast_to_native)
{
	bool canConvert = can_cast_to_native<FloatValue, float>::value;
	EXPECT_TRUE(canConvert);

	canConvert = can_cast_to_native<FloatValue, char>::value;
	EXPECT_FALSE(canConvert);
}

TEST(FloatValue, test_can_be_assigned)
{
	bool canAssign = can_be_assigned<FloatValue, float>::value;
	EXPECT_TRUE(canAssign);
	canAssign = can_be_assigned<FloatValue, int>::value;
	EXPECT_TRUE(canAssign);

	canAssign = can_be_assigned<FloatValue, char>::value;
	EXPECT_FALSE(canAssign);
}

TEST(FloatValue, test_accessing_as)
{
	FloatValue a(123.123f);
	EXPECT_EQ(get_value_as<float>(a), 123.123f);
	EXPECT_EQ(get_value_as<int>(a), 123);
}

TEST(FloatValue, test_setting_from)
{
	FloatValue a;
	set_value_from<float>(a, 1.0f);
	EXPECT_EQ(static_cast<float>(a), 1.0f);

	set_value_from<int>(a, 123);
	EXPECT_EQ(static_cast<float>(a), 123.0f);
}

TEST(Integer, test_constructions)
{
	EXPECT_EQ(0, static_cast<int>(Integer()));
	EXPECT_EQ(123, static_cast<int>(Integer(123)));
	EXPECT_EQ(123, static_cast<int>(Integer(123.123f)));
	EXPECT_EQ(123, static_cast<int>(Integer(FloatValue(123.123f))));
	EXPECT_EQ(123, static_cast<int>(Integer(std::make_shared<FloatValue>(123.123f))));
}

TEST(Integer, test_can_cast_to_native)
{
	bool canConvert = can_cast_to_native<Integer, int>::value;
	EXPECT_TRUE(canConvert);

	canConvert = can_cast_to_native<Integer, char>::value;
	EXPECT_FALSE(canConvert);
}

TEST(Integer, test_arithmetics)
{
	Integer a(123);
	Integer b(3);
	FloatValue c(3.5f);

	EXPECT_EQ(static_cast<int>(-a), -123);

	EXPECT_EQ(static_cast<int>(a + b), 123 + 3);
	EXPECT_EQ(static_cast<int>(a - b), 123 - 3);
	EXPECT_EQ(static_cast<int>(a * b), 123 * 3);
	EXPECT_EQ(static_cast<int>(a / b), 123 / 3);

	EXPECT_EQ(static_cast<float>(a + c), 123 + 3.5f);
	EXPECT_EQ(static_cast<float>(a - c), 123 - 3.5f);
	EXPECT_EQ(static_cast<float>(a * c), 123 * 3.5f);
	EXPECT_EQ(static_cast<float>(a / c), 123 / 3.5f);
}

TEST(Integer, test_accessing_as)
{
	Integer a(123);
	EXPECT_EQ(get_value_as<float>(a), 123.0f);
	EXPECT_EQ(get_value_as<int>(a), 123);
}

TEST(Integer, test_can_be_assigned)
{
	bool canAssign = can_be_assigned<Integer, int>::value;
	EXPECT_TRUE(canAssign);
	canAssign = can_be_assigned<Integer, float>::value;
	EXPECT_TRUE(canAssign);
}

TEST(Integer, test_setting_from)
{
	Integer a;
	set_value_from<int>(a, 1);
	EXPECT_EQ(static_cast<int>(a), 1);
	set_value_from<float>(a, 123.123f);
	EXPECT_EQ(static_cast<int>(a), 123);
}

TEST(Boolean, test_constructions)
{
	EXPECT_FALSE(static_cast<bool>(Boolean()));
	EXPECT_FALSE(static_cast<bool>(Boolean(false)));
	EXPECT_TRUE(static_cast<bool>(Boolean(true)));
}

TEST(Boolean, test_operations)
{
	Boolean a(false);
	Boolean b(true);

	EXPECT_FALSE(static_cast<bool>(a == b));
	EXPECT_TRUE(static_cast<bool>(a != b));
	EXPECT_TRUE(static_cast<bool>(!a));
}

TEST(Boolean, test_setting_from)
{
	Boolean a;
	set_value_from<bool>(a, true);
	EXPECT_TRUE(static_cast<bool>(a));
}

TEST(String, test_constructions)
{
	EXPECT_EQ("", static_cast<std::string>(String()));
	EXPECT_EQ("abc", static_cast<std::string>(String("abc")));
}

TEST(String, test_accessing_as)
{
	String a("abc");
	EXPECT_EQ(get_value_as<std::string>(a), "abc");
}

TEST(String, test_setting_from)
{
	String a;
	set_value_from<std::string>(a, "abc");
	EXPECT_EQ(static_cast<std::string>(a), "abc");
}

TEST(TypeInfo, test_runtime_types)
{
	EXPECT_EQ(TypeInfo::s_typeInfo, TypeInfo("type").GetTypeInfo());
	EXPECT_EQ(FloatValue::s_typeInfo, FloatValue(123.f).GetTypeInfo());
	EXPECT_EQ(Integer::s_typeInfo, Integer(123.f).GetTypeInfo());
	EXPECT_EQ(Boolean::s_typeInfo, Boolean(true).GetTypeInfo());
	EXPECT_EQ(String::s_typeInfo, String().GetTypeInfo());
	EXPECT_EQ(NullObject::s_typeInfo, NullObject().GetTypeInfo());
}
