#include <pagoda/dynamic/binding/make_member_function.h>
#include <pagoda/dynamic/boolean_value.h>

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

TEST(MakeMemberFunction, test_make_member_function)
{
	C c;
	std::function<void(C*)> f = make_member_function(&C::f);
	f(&c);
	ASSERT_TRUE(c.m_called);
}

TEST(MakeMemberFunction, test_make_member_function_with_args_and_return)
{
	C c;
	std::function<BooleanPtr(C*, BooleanPtr)> f = make_member_function(&C::f1);
	ASSERT_TRUE(static_cast<bool>(f(&c, std::make_shared<Boolean>(true))));
}

