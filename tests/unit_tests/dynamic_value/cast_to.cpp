#include <dynamic_value/binding/cast_to.h>
#include <dynamic_value/boolean_value.h>
#include <dynamic_value/float_value.h>

#include <dynamic_value/binding/call_free_function.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;

TEST(CastTo, test_dynamic_value_cast_to_native)
{
	BooleanPtr b = std::make_shared<Boolean>(true);
	bool nativeBool = cast_to<bool>(b);
	EXPECT_TRUE(nativeBool);
}

TEST(CastTo, test_base_dynamic_value_cast_to_dynamic_value)
{
	DynamicValueBasePtr b = std::make_shared<Boolean>(true);
	BooleanPtr dynamicBool = cast_to<Boolean>(b);
	EXPECT_TRUE(static_cast<bool>(*dynamicBool));
}
