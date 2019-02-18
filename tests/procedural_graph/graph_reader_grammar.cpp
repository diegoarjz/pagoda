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

TEST_F(GraphReaderGrammarTest, test_literal_integers)
{
	std::pair<std::string, int> literals[] = {{"123", 123}, {"0", 0}, {"-123", -123}};

	for (auto p : literals)
	{
		ast::literal l;
		std::string::const_iterator begin = std::begin(std::get<0>(p));
		std::string::const_iterator end = std::end(std::get<0>(p));
		bool r = qi::phrase_parse(begin, end, m_grammar.literal, qi::space, l);

		EXPECT_TRUE(r);
		EXPECT_EQ(boost::get<int>(l.m_value), std::get<1>(p));
	}
}

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
		EXPECT_EQ(boost::get<float>(l.m_value), std::get<1>(p));
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
		EXPECT_EQ(boost::get<std::string>(l.m_value), std::get<1>(p));
	}
}

TEST_F(GraphReaderGrammarTest, test_quoted_strings)
{
	std::string strings[] = {"\"A\"", "\"Abc\""};
	for (auto p : strings)
	{
		std::string s;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.quoted_string, qi::space, s);

		EXPECT_TRUE(r);
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
	std::string variables[] = {"abc", "_abc", "abc_", "ab_c", "_123", "_"};

	for (auto p : variables)
	{
		selector::ast::variable v;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.variable, qi::space, v);

		EXPECT_TRUE(r) << "Should have matched " << p;
		EXPECT_EQ(v.m_name, p);
	}
}

TEST_F(GraphReaderGrammarTest, test_parameter_value_literals)
{
	// std::string values[] = {"abc", "_abc", "abc_", "ab_c", "_123", "_"};
	std::string values[] = {"123", "123.0", "\"abc\""};

	for (auto p : values)
	{
		selector::ast::parameter_value v;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.parameter_value, qi::space, v);

		EXPECT_TRUE(r) << "Should have matched " << p;
		EXPECT_EQ(v.m_value.which(), 0);
	}
}

TEST_F(GraphReaderGrammarTest, test_parameter_value_variables)
{
	std::string values[] = {"abc", "_abc", "abc_", "ab_c", "_123", "_"};

	for (auto p : values)
	{
		selector::ast::parameter_value v;
		std::string::const_iterator begin = std::begin(p);
		std::string::const_iterator end = std::end(p);
		bool r = qi::phrase_parse(begin, end, m_grammar.parameter_value, qi::space, v);

		EXPECT_TRUE(r) << "Should have matched " << p;
		EXPECT_EQ(v.m_value.which(), 1);
	}
}

TEST_F(GraphReaderGrammarTest, test_parameter)
{
	std::string parameter = "name: \"value\"";

	selector::ast::parameter p;
	std::string::const_iterator begin = std::begin(parameter);
	std::string::const_iterator end = std::end(parameter);
	bool r = qi::phrase_parse(begin, end, m_grammar.parameter, qi::space, p);

	EXPECT_TRUE(r);
	EXPECT_EQ(p.m_name, "name");
}

TEST_F(GraphReaderGrammarTest, test_parameter_list)
{
	std::string parameter = "{par1: \"value\", par2: variable}";

	std::vector<selector::ast::parameter> p;
	std::string::const_iterator begin = std::begin(parameter);
	std::string::const_iterator end = std::end(parameter);
	bool r = qi::phrase_parse(begin, end, m_grammar.parameter_list, qi::space, p);

	EXPECT_TRUE(r);
	EXPECT_EQ(p.size(), 2);
}

TEST_F(GraphReaderGrammarTest, test_empty_parameter_list)
{
	std::string parameter = "{}";

	std::vector<selector::ast::parameter> p;
	std::string::const_iterator begin = std::begin(parameter);
	std::string::const_iterator end = std::end(parameter);
	bool r = qi::phrase_parse(begin, end, m_grammar.parameter_list, qi::space, p);

	EXPECT_TRUE(r);
	EXPECT_EQ(p.size(), 0);
}

TEST_F(GraphReaderGrammarTest, test_node_defition)
{
	std::string definition = "node_name = NodeType{par1: var, par2: 2, par3: 3.0, par4: \"4\"}";

	selector::ast::node_definition def;
	std::string::const_iterator begin = std::begin(definition);
	std::string::const_iterator end = std::end(definition);
	bool r = qi::phrase_parse(begin, end, m_grammar.node_definition, qi::space, def);

	EXPECT_TRUE(r);
	EXPECT_EQ(def.m_name, "node_name");
	EXPECT_EQ(def.m_type, "NodeType");
	EXPECT_EQ(def.m_nodeParameters.size(), 4);
}

TEST_F(GraphReaderGrammarTest, test_node_links)
{
	std::string link = "a -> b -> c";

	std::vector<std::string> links;
	std::string::const_iterator begin = std::begin(link);
	std::string::const_iterator end = std::end(link);
	bool r = qi::phrase_parse(begin, end, m_grammar.node_links, qi::space, links);

	EXPECT_TRUE(r);

	ASSERT_EQ(links.size(), 3);
	EXPECT_EQ(links[0], "a");
	EXPECT_EQ(links[1], "b");
	EXPECT_EQ(links[2], "c");
}

TEST_F(GraphReaderGrammarTest, test_instructions)
{
	std::string str = "n1 = NodeType{}\n"
	                  "n2 = NodeType{}\n"
	                  "n3 = NodeType{}\n"
	                  "n1 -> n2 -> n3";

	std::vector<selector::ast::instruction> instructions;
	std::string::const_iterator begin = std::begin(str);
	std::string::const_iterator end = std::end(str);
	bool r = qi::phrase_parse(begin, end, m_grammar, qi::space, instructions);

	EXPECT_TRUE(r);
	EXPECT_EQ(instructions.size(), 4);
}
