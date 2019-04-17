#include <parameter/enumeration_parameter.h>
#include <parameter/float_parameter.h>
#include <parameter/parameter.h>
#include <parameter/parameter_identifier.h>
#include <parameter/string_parameter.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(ParameterTest, FloatParameterCreation)
{
	auto par = std::make_shared<Parameter<FloatParameter>>("floatPar");

	EXPECT_EQ(par->Name(), "floatPar");
	EXPECT_EQ(par->GetValue(), 0.0f);
}

TEST(ParameterTest, FloatParameterGetterSetter)
{
	auto par = std::make_shared<Parameter<FloatParameter>>("floatPar");
	par->Set<FloatParameter>(123.5);
	EXPECT_EQ(par->Get<FloatParameter>(), 123.5);
}

TEST(ParameterTest, ConstFloatParameterGetterSetter)
{
	std::shared_ptr<ParameterBase> par = std::make_shared<Parameter<FloatParameter>>("floatPar");
	const auto cPar = par;
	cPar->Set<FloatParameter>(123.5);
	EXPECT_EQ(cPar->Get<FloatParameter>(), 123.5);
}

TEST(ParameterTest, StringParameterCreation)
{
	auto par = std::make_shared<Parameter<StringParameter>>("stringPar");
	EXPECT_EQ(par->GetValue(), "");
}

TEST(ParameterTest, StringParameterSetter)
{
	auto par = std::make_shared<Parameter<StringParameter>>("stringPar");
	par->Set<StringParameter>("new_value");
	EXPECT_EQ(par->GetValue(), "new_value");
}

struct ParameterTypesEnumeration
{
	using Values = ParameterTypes;
};
TEST(ParameterTest, EnumParameterCreation)
{
	auto par = std::make_shared<Parameter<EnumerationParameter<ParameterTypesEnumeration>>>("enumPar");
	EXPECT_EQ(par->GetValue(), ParameterTypes::Float);
}

TEST(ParameterTest, EnumParameterSetter)
{
	auto par = std::make_shared<Parameter<EnumerationParameter<ParameterTypesEnumeration>>>("enumPar");
	par->Set<EnumerationParameter<ParameterTypesEnumeration>>(ParameterTypes::String);
	EXPECT_EQ(par->GetValue(), ParameterTypes::String);
}
