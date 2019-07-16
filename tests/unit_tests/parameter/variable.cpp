#include <parameter/variable.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(VariableTest, when_creating_a_variable_with_a_single_identifier_should_store_the_identifier)
{
	Variable v("abc");
	ASSERT_EQ(v.GetIdentifiers(), std::list<std::string>{"abc"});
}

TEST(VariableTest, when_creating_a_variable_with_multiple_identifiers_should_store_all_identifiers)
{
	Variable v("a.b.c");
	ASSERT_EQ(v.GetIdentifiers(), std::list<std::string>{"a", "b", "c"});
}

TEST(VariableTest, when_getting_the_string_with_multiple_identifiers_should_connect_the_identifiers_with_dot)
{
	Variable v({"a", "b", "c"});
	ASSERT_EQ(v.ToString(), "a.b.c");
}

TEST(VariableTest, when_comparing_with_a_string_should_use_the_string_representation)
{
	Variable v({"a", "b", "c"});
	EXPECT_EQ(v, "a.b.c");
	EXPECT_NE(v, "a.b");
}

TEST(VariableTest, when_adding_identifiers_should_store_them)
{
	Variable v("a");
	v.AddIdentifier("b");
	EXPECT_EQ(v, "a.b");
}

TEST(VariableTest, when_checking_if_compound_should_return_true_if_more_than_one_identifier)
{
	Variable v("a");
	EXPECT_FALSE(v.IsCompound());
	v.AddIdentifier("b");
	EXPECT_TRUE(v.IsCompound());
}