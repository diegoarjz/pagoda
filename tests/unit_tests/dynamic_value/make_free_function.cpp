#include <dynamic_value/binding/make_free_function.h>
#include <dynamic_value/boolean_value.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace selector;

static bool called_f = false;
static void f() { called_f = true; }

static BooleanPtr f2(const BooleanPtr &b) { return b; }

TEST(MakeFreeFunction, test_make_free_function)
{
	std::function<void(void)> function = make_free_function(f);
	function();
	EXPECT_TRUE(called_f);
}

TEST(MakeFreeFunction, test_make_free_function_with_args_and_return)
{
	std::function<BooleanPtr(const BooleanPtr &)> function = make_free_function(f2);
	EXPECT_TRUE(static_cast<bool>(function(std::make_shared<Boolean>(true))));
}
