#include <pagoda/dynamic/binding/make_member_function.h>
#include <pagoda/dynamic/boolean_value.h>

#include <pagoda/dynamic/binding/call_member_function.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;
using namespace pagoda::dynamic;

class C
{
public:
	C() : m_called(false) {}

	void f() { m_called = true; }
	BooleanPtr f1(BooleanPtr b) { return b; }

	bool m_called;
};

TEST(CallMemberFunction, test_call_no_args_return_void)
{
	C c;
	auto f = make_member_function(&C::f);
	call_member_function(c, f, {});
	ASSERT_TRUE(c.m_called);
}

TEST(CallMemberFunction, test_call_args_and_return)
{
	C c;
	auto f = make_member_function(&C::f1);
	ASSERT_TRUE(static_cast<bool>(call_member_function(c, f, {std::make_shared<Boolean>(true)})));
}
