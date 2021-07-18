#include "pagoda/objects/parameter_creator.h"

#include <gtest/gtest.h>

using namespace pagoda;
using namespace pagoda::objects;

TEST(ParameterCreatorTest, test_string_creation)
{
	bool called{false};
	ParameterCreator c([&called](std::shared_ptr<ParameterBase> p) {
		called = true;
		return true;
	});
	std::string v;
	auto par = c.StringParameter(&v, "par", "Label", "abc");
	ASSERT_NE(par, nullptr);
	EXPECT_EQ(par->GetValue(), "abc");
	EXPECT_EQ(par->GetName(), "par");
	EXPECT_EQ(par->GetLabel(), "Label");
	EXPECT_TRUE(called);
}

TEST(ParameterCreatorTest, test_float_creation)
{
	bool called{false};
	ParameterCreator c([&called](std::shared_ptr<ParameterBase> p) {
		called = true;
		return true;
	});
	float v;
	auto par = c.FloatParameter(&v, "par", "Label", 123.0);
	ASSERT_NE(par, nullptr);
	EXPECT_EQ(par->GetValue(), 123.0);
	EXPECT_EQ(par->GetName(), "par");
	EXPECT_EQ(par->GetLabel(), "Label");
	EXPECT_TRUE(called);
}

TEST(ParameterCreatorTest, test_int_creation)
{
	bool called{false};
	ParameterCreator c([&called](std::shared_ptr<ParameterBase> p) {
		called = true;
		return true;
	});
	int v;
	auto par = c.IntegerParameter(&v, "par", "Label", 123);
	ASSERT_NE(par, nullptr);
	EXPECT_EQ(par->GetValue(), 123);
	EXPECT_EQ(par->GetName(), "par");
	EXPECT_EQ(par->GetLabel(), "Label");
	EXPECT_TRUE(called);
}

TEST(ParameterCreatorTest, test_bool_creation)
{
	bool called{false};
	ParameterCreator c([&called](std::shared_ptr<ParameterBase> p) {
		called = true;
		return true;
	});
	bool v;
	auto par = c.BooleanParameter(&v, "par", "Label", true);
	ASSERT_NE(par, nullptr);
	EXPECT_EQ(par->GetValue(), true);
	EXPECT_EQ(par->GetName(), "par");
	EXPECT_EQ(par->GetLabel(), "Label");
	EXPECT_TRUE(called);
}

TEST(ParameterCreatorTest, test_plane_creation)
{
	bool called{false};
	ParameterCreator c([&called](std::shared_ptr<ParameterBase> p) {
		called = true;
		return true;
	});
	math::Plane<float> v;
	auto par =
	  c.PlaneParameter(&v, "par", "Label", math::Plane<float>{{1, 0, 0}, 1});
	ASSERT_NE(par, nullptr);
	EXPECT_EQ(par->GetValue(), (math::Plane<float>{{1, 0, 0}, 1}));
	EXPECT_EQ(par->GetName(), "par");
	EXPECT_EQ(par->GetLabel(), "Label");
	EXPECT_TRUE(called);
}

TEST(ParameterCreatorTest, return_null_if_callback_returns_false)
{
	ParameterCreator c([](std::shared_ptr<ParameterBase> p) { return false; });
	float f;
	EXPECT_EQ(c.FloatParameter(&f, "par", "Label"), nullptr);
}
