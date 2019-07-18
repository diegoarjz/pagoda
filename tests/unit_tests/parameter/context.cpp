#include <parameter/context.h>
#include <parameter/parameter.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(ContextTest, test_creation)
{
	auto c1 = std::make_shared<Context>("parent_context");

	EXPECT_EQ(c1->Name(), "parent_context");
}

TEST(ContextTest, test_set_parent)
{
	auto c1 = std::make_shared<Context>("parent_context");
	auto c2 = std::make_shared<Context>("child_context");

	c2->SetParent(c1);

	EXPECT_EQ(c2->Parent(), c1);
	ASSERT_EQ(c1->Children().size(), 1);
	EXPECT_EQ(c1->Children().at("child_context"), c2);
}

TEST(ContextTest, test_set_new_parent)
{
	auto c1 = std::make_shared<Context>("parent_context");
	auto c2 = std::make_shared<Context>("child_context");
	auto c3 = std::make_shared<Context>("new_parent");

	c2->SetParent(c1);
	c2->SetParent(c3);

	EXPECT_EQ(c1->Children().size(), 0);
	EXPECT_EQ(c2->Parent(), c3);
	ASSERT_EQ(c3->Children().size(), 1);
	EXPECT_EQ(c3->Children().at("child_context"), c2);
}

TEST(ContextTest, test_set_new_parent_context_already_has_child_with_same_name)
{
	auto c1 = std::make_shared<Context>("parent_context");
	auto c2 = std::make_shared<Context>("child_context");
	auto c3 = std::make_shared<Context>("child_context");

	EXPECT_TRUE(c2->SetParent(c1));
	EXPECT_FALSE(c3->SetParent(c1));
}

TEST(ContextTest, test_add_parameter)
{
	Context c("a_context");

	auto par = c.CreateParameter<float>("floatPar", 1.0f);
	EXPECT_EQ(std::get<float>(par), 1.0f);
}

TEST(ContextTest, test_get_parameter)
{
	Context c("a_context");

	auto par = c.CreateParameter<float>("floatPar", 1.0f);
	ASSERT_EQ(c.GetParameter("floatPar"), par);
}

TEST(ContextTest, test_get_sub_context)
{
	auto c1 = std::make_shared<Context>("parent_context");
	auto c2 = std::make_shared<Context>("child_context");
	c2->SetParent(c1);

	ASSERT_EQ(c1->GetSubContext("child_context"), c2);
}

TEST(ContextTest, test_get_sub_context_non_existing)
{
	Context c("a_context");

	ASSERT_EQ(c.GetSubContext("a_non_existing_context"), nullptr);
}

TEST(ContextTest, when_resolving_a_simple_variable_should_return_the_parameter_value)
{
	Context c("c");
	c->SetParameter("a", 123.0f);
	auto val = c->ResolveVariable(Variable("a"));
	EXPECT_EQ(val, 123.0f);
}

TEST(ContextTest, when_resolving_a_compound_variable_should_traverse_the_hierarchy)
{
	Context c1("c1");
	Context c2("c2");
	c1->SetParameter("c2", c2);
	c2->SetParameter("a", 123.0f);
	auto val = c1->ResolveVariable(Variable("c2.a"));
	EXPECT_EQ(val, 123.0f);
}