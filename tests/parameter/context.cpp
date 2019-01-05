#include <parameter/context.h>
#include <parameter/float_parameter.h>
#include <parameter/parameter.h>
#include <parameter/parameter_identifier.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(ContextTest, test_creation)
{
	auto c1 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("parent_context").second);

	EXPECT_EQ(c1->Name(), "parent_context");
}

TEST(ContextTest, test_set_parent)
{
	auto c1 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("parent_context").second);
	auto c2 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("child_context").second);

	c2->SetParent(c1);

	EXPECT_EQ(c2->Parent(), c1);
	ASSERT_EQ(c1->Children().size(), 1);
	EXPECT_EQ(c1->Children().at("child_context"), c2);
}

TEST(ContextTest, test_set_new_parent)
{
	auto c1 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("parent_context").second);
	auto c2 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("child_context").second);
	auto c3 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("new_parent").second);

	c2->SetParent(c1);
	c2->SetParent(c3);

	EXPECT_EQ(c1->Children().size(), 0);
	EXPECT_EQ(c2->Parent(), c3);
	ASSERT_EQ(c3->Children().size(), 1);
	EXPECT_EQ(c3->Children().at("child_context"), c2);
}

TEST(ContextTest, test_set_new_parent_context_already_has_child_with_same_name)
{
	auto c1 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("parent_context").second);
	auto c2 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("child_context").second);
	auto c3 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("child_context").second);

	EXPECT_TRUE(c2->SetParent(c1));
	EXPECT_FALSE(c3->SetParent(c1));
}

TEST(ContextTest, test_add_parameter)
{
	Context c(ParameterIdentifier::CreateIdentifier("a_context").second);

	auto par = c.CreateParameter<FloatParameter>("floatPar", 1.0f);
	ASSERT_NE(par, nullptr);
	EXPECT_EQ(par->Name(), "floatPar");
	EXPECT_EQ(par->GetValue(), 1.0f);
}

TEST(ContextTest, test_try_add_param_same_name)
{
	Context c(ParameterIdentifier::CreateIdentifier("a_context").second);

	c.CreateParameter<FloatParameter>("floatPar", 1.0f);
	ASSERT_EQ(c.CreateParameter<FloatParameter>("floatPar", 1.0f), nullptr);
}

TEST(ContextTest, test_get_parameter)
{
	Context c(ParameterIdentifier::CreateIdentifier("a_context").second);

	auto par = c.CreateParameter<FloatParameter>("floatPar", 1.0f);
	ASSERT_EQ(c.GetParameter("floatPar"), par);
}

TEST(ContextTest, test_get_sub_context)
{
	auto c1 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("parent_context").second);
	auto c2 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("child_context").second);
	c2->SetParent(c1);

	ASSERT_EQ(c1->GetSubContext("child_context"), c2);
}

TEST(ContextTest, test_get_sub_context_non_existing)
{
	Context c(ParameterIdentifier::CreateIdentifier("a_context").second);

	ASSERT_EQ(c.GetSubContext("a_non_existing_context"), nullptr);
}

TEST(ContextTest, test_parameters_should_have_different_names_from_sub_contexts)
{
	auto c1 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("parent").second);
	auto c2 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("a_name").second);

	c2->SetParent(c1);

	EXPECT_EQ(c1->CreateParameter<FloatParameter>("a_name", 1.0f), nullptr);
}

TEST(ContextTest, test_sub_contexts_should_have_different_names_from_parameters)
{
	auto c1 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("parent").second);
	auto c2 = std::make_shared<Context>(ParameterIdentifier::CreateIdentifier("a_name").second);

	auto par = c1->CreateParameter<FloatParameter>("a_name", 1.0f);
	EXPECT_FALSE(c2->SetParent(c1));
}
