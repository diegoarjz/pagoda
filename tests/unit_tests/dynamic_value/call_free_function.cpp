#include <pagoda/dynamic_value/binding/make_free_function.h>
#include <pagoda/dynamic_value/boolean_value.h>

#include <pagoda/dynamic_value/binding/call_free_function.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;

static bool called_f = false;
static void f() { called_f = true; }

static BooleanPtr f2(BooleanPtr b) { return b; }

TEST(CallFreeFunction, test_call_no_args_return_void)
{
	std::function<void(void)> function = make_free_function(f);
	call_free_function(function, {});
	EXPECT_TRUE(called_f);
}

TEST(CallFreeFunction, test_call_args_and_return)
{
	std::function<BooleanPtr(BooleanPtr)> function = make_free_function(f2);
	EXPECT_TRUE(call_free_function(function, {std::make_shared<Boolean>(true)}));
}
