#include <parameter/parameter_identifier.h>

#include <gtest/gtest.h>

using namespace selector;

TEST(ParameterIdentifierIsValid, valid)
{
	std::vector<std::string> values{"a",
	                                "abc",
	                                "a_parameter",
	                                "_another_parameter",
	                                "_ends_with_underscore_",
	                                "has_numbers_123",
	                                "_123_in_the_beginning"};

	for (auto s : values)
	{
		EXPECT_TRUE(ParameterIdentifier::IsValidIdentifier(s));
	}
}

TEST(ParameterIdentifierIsValid, invalid)
{
	std::vector<std::string> values{"", "1", "with spaces", "1_numbers_in_the_beginning", "!\"#$%&/()="};

	for (auto s : values)
	{
		EXPECT_FALSE(ParameterIdentifier::IsValidIdentifier(s));
	}
}

TEST(ParameterIdentifierTest, create_valid_identifier)
{
	auto identifier = ParameterIdentifier::CreateIdentifier("a_valid_identifier_123");

	EXPECT_EQ(identifier.first, true);
	EXPECT_EQ(identifier.second.GetIdentifier(), "a_valid_identifier_123");
}

TEST(ParameterIdentifierTest, create_invalid_identifier)
{
	auto identifier = ParameterIdentifier::CreateIdentifier("123_an_invalid_identifier_123");

	EXPECT_EQ(identifier.first, false);
	EXPECT_EQ(identifier.second.GetIdentifier(), "");
}
