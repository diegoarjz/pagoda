#include "pagoda/objects/parameter.h"

#include "pagoda/math/plane.h"

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::dynamic;
using namespace pagoda::objects;

template<typename T>
class ParameterTest : public ::testing::Test
{
	protected:
	void SetUp()
	{
	}
	void TearDown()
	{
	}
};

namespace
{
template<typename T>
struct defaultValues
{
	static const T value1;
	static const T value2;
	static dynamic::DynamicValueBasePtr getDynamicValue();
	static const std::string expression1;
	static const std::string strValue;
};

template<>
const std::string defaultValues<std::string>::value1{"a string"};
template<>
const std::string defaultValues<std::string>::value2{"another string"};
template<>
DynamicValueBasePtr defaultValues<std::string>::getDynamicValue()
{
	return std::make_shared<String>(value1);
}
template<>
const std::string defaultValues<std::string>::expression1{"\"a string\";"};
template<>
const std::string defaultValues<std::string>::strValue{"a string"};

template<>
const float defaultValues<float>::value1{123.456f};
template<>
const float defaultValues<float>::value2{321.987f};
template<>
DynamicValueBasePtr defaultValues<float>::getDynamicValue()
{
	return std::make_shared<FloatValue>(value1);
}
template<>
const std::string defaultValues<float>::expression1{"123.456;"};
template<>
const std::string defaultValues<float>::strValue{"123.456"};

template<>
const int defaultValues<int>::value1{123};
template<>
const int defaultValues<int>::value2{321};
template<>
DynamicValueBasePtr defaultValues<int>::getDynamicValue()
{
	return std::make_shared<Integer>(value1);
}
template<>
const std::string defaultValues<int>::expression1{"123;"};
template<>
const std::string defaultValues<int>::strValue{"123"};

template<>
const bool defaultValues<bool>::value1{false};
template<>
const bool defaultValues<bool>::value2{true};
template<>
DynamicValueBasePtr defaultValues<bool>::getDynamicValue()
{
	return std::make_shared<Boolean>(value1);
}
template<>
const std::string defaultValues<bool>::expression1{"false;"};
template<>
const std::string defaultValues<bool>::strValue{"false"};

template<>
const math::Plane<float> defaultValues<math::Plane<float>>::value1{{1, 0, 0},
                                                                   1};
template<>
const math::Plane<float> defaultValues<math::Plane<float>>::value2{
  {2.0f, 3.0f, 4.0f}, 5.0f};
template<>
DynamicValueBasePtr defaultValues<math::Plane<float>>::getDynamicValue()
{
	return std::make_shared<DynamicPlane>(value1);
}
template<>
const std::string defaultValues<math::Plane<float>>::expression1{
  "Plane(Vector3(1,2,3), Vector3(1,0,0));"};
template<>
const std::string defaultValues<math::Plane<float>>::strValue{"[1, 0, 0], 1"};
}  // namespace

TYPED_TEST_SUITE_P(ParameterTest);

TYPED_TEST_P(ParameterTest, test_constructors)
{
	TypeParam v = defaultValues<TypeParam>::value1;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");
	EXPECT_EQ(par->GetName(), "par");
	EXPECT_EQ(par->GetLabel(), "par");

	Parameter<TypeParam> par2(&v, "par2", "label");
	EXPECT_EQ(par2.GetName(), "par2");
	EXPECT_EQ(par2.GetLabel(), "label");
}

TYPED_TEST_P(ParameterTest, test_getters)
{
	TypeParam v = defaultValues<TypeParam>::value1;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");
	EXPECT_EQ(par->GetValue(), defaultValues<TypeParam>::value1);

	const Parameter<TypeParam> par2(&v, "par");
	EXPECT_EQ(par2.GetValue(), defaultValues<TypeParam>::value1);
}

TYPED_TEST_P(ParameterTest, test_setters)
{
	TypeParam v = defaultValues<TypeParam>::value1;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");

	bool called{false};
	par->OnChanged(
	  [&called](std::shared_ptr<ParameterBase> p) { called = true; });

	par->SetValue(defaultValues<TypeParam>::value2);
	EXPECT_EQ(v, defaultValues<TypeParam>::value2);
	EXPECT_TRUE(called);

	called = false;
	par->SetValue(std::move(defaultValues<TypeParam>::value1));
	EXPECT_EQ(v, defaultValues<TypeParam>::value1);
	EXPECT_TRUE(called);
}

TYPED_TEST_P(ParameterTest, test_flags)
{
	TypeParam v = defaultValues<TypeParam>::value1;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");

	for (std::size_t f = 0;
	     f < static_cast<std::size_t>(ParameterBase::Flag::FlagCount); ++f) {
		auto flag = static_cast<ParameterBase::Flag>(f);

		EXPECT_FALSE(par->HasFlag(flag));
		par->SetFlag(flag, true);
		EXPECT_TRUE(par->HasFlag(flag));
		par->SetFlag(flag, false);
		EXPECT_FALSE(par->HasFlag(flag));
	}
}

TYPED_TEST_P(ParameterTest, test_conversion_from_dynamic_value)
{
	TypeParam v = defaultValues<TypeParam>::value2;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");
	bool called{false};
	par->OnChanged(
	  [&called](std::shared_ptr<ParameterBase> p) { called = true; });

	par->FromDynamicValue(defaultValues<TypeParam>::getDynamicValue());
	EXPECT_EQ(v, defaultValues<TypeParam>::value1);
	EXPECT_TRUE(called);
}

TYPED_TEST_P(ParameterTest, test_conversion_to_dynamic_value)
{
	TypeParam v = defaultValues<TypeParam>::value1;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");
	par->SetValue(defaultValues<TypeParam>::value2);
	auto dynamicValue = par->ToDynamicValue();
	ASSERT_NE(dynamicValue, nullptr);
}

TYPED_TEST_P(ParameterTest, test_parameters_with_expressions)
{
	TypeParam v = defaultValues<TypeParam>::value2;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");
	bool called{false};
	par->OnChanged(
	  [&called](std::shared_ptr<ParameterBase> p) { called = true; });

	par->SetExpression(defaultValues<TypeParam>::expression1);
	ASSERT_TRUE(par->HasExpression());
	EXPECT_EQ(par->GetExpression(), defaultValues<TypeParam>::expression1);

	par->EvaluateExpression();
	EXPECT_EQ(v, defaultValues<TypeParam>::value1);
	EXPECT_TRUE(called);
}

TYPED_TEST_P(ParameterTest, test_conversion_to_and_from_string)
{
	TypeParam v = defaultValues<TypeParam>::value2;
	auto par = std::make_shared<Parameter<TypeParam>>(&v, "par");
	bool called{false};
	par->OnChanged(
	  [&called](std::shared_ptr<ParameterBase> p) { called = true; });

	par->FromString(defaultValues<TypeParam>::strValue);
	ASSERT_TRUE(called);
	EXPECT_EQ(par->GetValue(), defaultValues<TypeParam>::value1);
}

// clang-format off
REGISTER_TYPED_TEST_SUITE_P(ParameterTest,
                            test_constructors,
                            test_getters,
                            test_setters,
                            test_flags,
                            test_conversion_from_dynamic_value,
                            test_conversion_to_dynamic_value,
                            test_parameters_with_expressions,
                            test_conversion_to_and_from_string);

using MyTypes = ::testing::Types<std::string,
                                 float,
                                 int,
                                 bool,
                                 math::Plane<float>>;
// clang-format on
INSTANTIATE_TYPED_TEST_SUITE_P(Instantiated, ParameterTest, MyTypes);
