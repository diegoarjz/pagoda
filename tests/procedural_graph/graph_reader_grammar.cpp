#include <procedural_graph/graph_reader_grammar.h>

#include <boost/spirit/include/qi.hpp>

#include <gtest/gtest.h>

using namespace selector;
using namespace boost::spirit;

class GraphReaderGrammarTest : public ::testing::Test
{
protected:
	void SetUp() {}
	void TearDown() {}

	GraphReaderGrammar<std::string::const_iterator> m_grammar;
};

TEST_F(GraphReaderGrammarTest, test_literal_floats)
{
	std::pair<std::string, float> literals[] = {{"123.0", 123.0}, {"0.0", 0.0}, {"-123.0", -123.0}};

	for (auto p : literals)
	{
		ast::literal l;
		std::string::const_iterator begin = std::begin(std::get<0>(p));
		std::string::const_iterator end = std::end(std::get<0>(p));
		bool r = qi::phrase_parse(begin, end, m_grammar.literal, qi::space, l);

		EXPECT_TRUE(r);
		EXPECT_EQ(begin, end);
		EXPECT_EQ(boost::get<float>(l), std::get<1>(p));
	}
}

TEST_F(GraphReaderGrammarTest, test_literal_strings)
{
	std::pair<std::string, std::string> literals[] = {{"\"A\"", "A"}};

	for (auto p : literals)
	{
		ast::literal l;
		std::string::const_iterator begin = std::begin(std::get<0>(p));
		std::string::const_iterator end = std::end(std::get<0>(p));
		bool r = qi::phrase_parse(begin, end, m_grammar.literal, qi::space, l);

		EXPECT_TRUE(r);
		EXPECT_EQ(begin, end);
		EXPECT_EQ(boost::get<std::string>(l), std::get<1>(p));
	}
}

TEST_F(GraphReaderGrammarTest, test_literal_failure)
{
	std::string literals[] = {"A", "-a12"};

	for (auto p : literals)
	{
		ast::literal l;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.literal, qi::space, l);

		EXPECT_FALSE(r) << "Should not have matched " << p;
	}
}

TEST_F(GraphReaderGrammarTest, test_identifier)
{
	std::string identifiers[] = {"abc", "_abc", "abc_", "ab_c", "_123", "_"};

	for (auto p : identifiers)
	{
		std::string i;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.identifier, qi::space, i);

		EXPECT_TRUE(r) << "Should have matched " << p;
		EXPECT_EQ(begin, end);
		EXPECT_EQ(i, p);
	}
}

TEST_F(GraphReaderGrammarTest, test_identifier_fail)
{
	std::string identifiers[] = {"1", "123", "1abc", "-"};

	for (auto p : identifiers)
	{
		std::string i;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.identifier, qi::space, i);

		EXPECT_FALSE(r) << "Should not have matched " << p;
	}
}

TEST_F(GraphReaderGrammarTest, test_variable)
{
	selector::ast::variable var;
	std::string v = "variable_name";
	std::string::const_iterator begin = std::begin(v);
	std::string::const_iterator end = std::end(v);
	bool r = qi::phrase_parse(begin, end, m_grammar.identifier, qi::space, var);

	EXPECT_TRUE(r);
	EXPECT_EQ(begin, end);
	EXPECT_EQ(var.m_name, v);
}

TEST_F(GraphReaderGrammarTest, test_expressions_valid)
{
	// clang-format off
    std::string expressions[] = {
        "1", "a", "\"b\"",
        "1+1", "1-1", "1*1", "1/1",
        "\"a\"+1", "\"a\"+\"a\"",
        "a+1", "a*1", "a+a",
        "1+1+1", "1*1+1*1", "1*(1+1)*1"
    };
	// clang-format on

	for (auto p : expressions)
	{
		selector::ast::expression i;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.expression, qi::space, i);

		EXPECT_TRUE(r) << "Should have matched " << p;
		EXPECT_EQ(begin, end);
	}
}

TEST_F(GraphReaderGrammarTest, test_expressions_invalid)
{
	// clang-format off
    std::string expressions[] = {
        "", "-", "+", "*", "/", "()"
    };
	// clang-format on

	for (auto p : expressions)
	{
		selector::ast::expression i;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.expression, qi::space, i);

		EXPECT_FALSE(r) << "Should not have matched " << p;
	}
}

TEST_F(GraphReaderGrammarTest, test_assignment)
{
	selector::ast::assignment assignment;
	std::string s = "a = b";

	std::string::const_iterator begin = std::begin(s);
	std::string::const_iterator end = std::end(s);
	bool r = qi::phrase_parse(begin, end, m_grammar.assignment, qi::space, assignment);

	EXPECT_TRUE(r);
	EXPECT_EQ(begin, end);
	EXPECT_EQ(assignment.m_lhs.m_name, "a");
}

TEST_F(GraphReaderGrammarTest, test_named_arg)
{
	selector::ast::named_arg named_arg;
	std::string s = "a: 1";

	std::string::const_iterator begin = std::begin(s);
	std::string::const_iterator end = std::end(s);
	bool r = qi::phrase_parse(begin, end, m_grammar.named_arg, qi::space, named_arg);

	EXPECT_TRUE(r);
	EXPECT_EQ(begin, end);
	EXPECT_EQ(named_arg.m_name, "a");
	EXPECT_EQ(named_arg.m_value.which(), 0);
}

TEST_F(GraphReaderGrammarTest, test_node_definition)
{
	selector::ast::node_definition node_def;
	std::string s = "Operation(type: \"Extrude\"){amount: 10}";

	std::string::const_iterator begin = std::begin(s);
	std::string::const_iterator end = std::end(s);
	bool r = qi::phrase_parse(begin, end, m_grammar.node_definition, qi::space, node_def);

	EXPECT_TRUE(r);
	EXPECT_EQ(begin, end);
	EXPECT_EQ(node_def.m_nodeType, "Operation");
	EXPECT_EQ(node_def.m_constructionArguments.size(), 1);
	EXPECT_EQ(node_def.m_nodeParameters.size(), 1);
}

TEST_F(GraphReaderGrammarTest, test_node_definition_assignment)
{
	selector::ast::assignment node_assignment;
	std::string s = "n = Operation(type: \"Extrude\"){amount: 10}";

	std::string::const_iterator begin = std::begin(s);
	std::string::const_iterator end = std::end(s);
	bool r = qi::phrase_parse(begin, end, m_grammar.assignment, qi::space, node_assignment);

	EXPECT_TRUE(r);
	EXPECT_EQ(begin, end);
	EXPECT_EQ(node_assignment.m_lhs.m_name, "n");
}
