#include <pagoda/dynamic/binding/cast_to.h>
#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>

#include <pagoda/dynamic/binding/call_free_function.h>

#include <gtest/gtest.h>

#include "../test_utils.h"

using namespace pagoda;
using namespace pagoda::dynamic;

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
