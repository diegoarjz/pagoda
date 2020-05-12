#include <pagoda/dynamic_value/binding/call_free_function.h>
#include <pagoda/dynamic_value/binding/make_free_function.h>
#include <pagoda/dynamic_value/binding/make_member_function.h>
#include <pagoda/dynamic_value/binding/return_from_function.h>
#include <pagoda/dynamic_value/binding/return_from_member_function.h>
#include <pagoda/dynamic_value/boolean_value.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;

static BooleanPtr f2(BooleanPtr b) { return b; }

TEST(ReturnFromFunction, test_return_from_function)
{
	std::function<BooleanPtr(BooleanPtr)> function = make_free_function(f2);
	try
	{
		return_from_function(function, {std::make_shared<Boolean>(true)});
	}
	catch (DynamicValueBasePtr &returnValue)
	{
		auto b = std::dynamic_pointer_cast<Boolean>(returnValue);
		ASSERT_NE(b, nullptr);
		ASSERT_TRUE(static_cast<bool>(b));
		return;
	}
	FAIL() << "Should have thrown";
}

class D
{
public:
	D() : m_called(false) {}

	BooleanPtr f1(BooleanPtr b)
	{
		m_called = static_cast<bool>(b);
		return b;
	}

	bool m_called;
};

TEST(ReturnFromMemberFunction, test_return_from_member_function)
{
	D c;
	auto function = make_member_function(&D::f1);
	try
	{
		return_from_member_function(c, function, {std::make_shared<Boolean>(true)});
	}
	catch (DynamicValueBasePtr &returnValue)
	{
		auto b = std::dynamic_pointer_cast<Boolean>(returnValue);
		ASSERT_NE(b, nullptr);
		EXPECT_TRUE(static_cast<bool>(b));
		EXPECT_TRUE(c.m_called);
		return;
	}
	FAIL() << "Should have thrown";
}
