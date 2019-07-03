#include <procedural_graph/reader/graph_reader_grammar.h>
#include <procedural_graph/reader/named_argument.h>
#include <procedural_graph/reader/node_definition_node.h>
#include <procedural_graph/reader/node_link_node.h>

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
		std::string in = std::get<0>(p);
		boost::variant<std::string, float> l;
		std::string::const_iterator begin = std::begin(in);
		std::string::const_iterator end = std::end(in);
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
		boost::variant<std::string, float> l;
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
		boost::variant<std::string, float> l;
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

TEST_F(GraphReaderGrammarTest, test_expression)
{
	std::pair<std::string, std::string> expressions[] = {{"$<a;>$", "a;"}, {"$<1+2;>$", "1+2;"}};

	for (auto e : expressions)
	{
		std::string out;
		std::string::const_iterator begin = std::begin(std::get<0>(e));
		std::string::const_iterator end = std::end(std::get<0>(e));
		bool r = qi::phrase_parse(begin, end, m_grammar.expression, qi::space, out);

		EXPECT_TRUE(r) << "Should have matched " << std::get<0>(e);
		EXPECT_EQ(begin, end);
		EXPECT_EQ(out, std::get<1>(e));
	}
}

TEST_F(GraphReaderGrammarTest, test_named_simple_arg)
{
	std::string args[] = {"a : \"abc\"", "b:123.0"};

	for (auto a : args)
	{
		NamedArgumentPtr out;
		std::string::const_iterator begin = std::begin(a);
		std::string::const_iterator end = std::end(a);
		bool r = qi::phrase_parse(begin, end, m_grammar.named_simple_arg, qi::space, out);

		EXPECT_TRUE(r) << "Should have matched " << a;
		EXPECT_EQ(begin, end);
	}
}

TEST_F(GraphReaderGrammarTest, test_named_simple_arg_construction_string)
{
	NamedArgumentPtr out;
	std::string a = "a : \"abc\"";
	std::string::const_iterator begin = std::begin(a);
	std::string::const_iterator end = std::end(a);
	qi::phrase_parse(begin, end, m_grammar.named_simple_arg, qi::space, out);

	ASSERT_NE(out, nullptr);
	EXPECT_EQ(out->GetName(), "a");
	EXPECT_EQ(out->GetArgumentType(), NamedArgument::ArgumentType::String);
	EXPECT_EQ(out->GetArgumentValue(), "abc");
}

TEST_F(GraphReaderGrammarTest, test_named_simple_arg_construction_float)
{
	NamedArgumentPtr out;
	std::string a = "a : 123";
	std::string::const_iterator begin = std::begin(a);
	std::string::const_iterator end = std::end(a);
	qi::phrase_parse(begin, end, m_grammar.named_simple_arg, qi::space, out);

	ASSERT_NE(out, nullptr);
	EXPECT_EQ(out->GetName(), "a");
	EXPECT_EQ(out->GetArgumentType(), NamedArgument::ArgumentType::Float);
	EXPECT_EQ(std::atof(out->GetArgumentValue().c_str()), 123.0f);
}

TEST_F(GraphReaderGrammarTest, test_construction_args)
{
	std::string args[] = {"", "a : \"abc\", b:123.0", "b:123.0"};
	std::size_t expectedSizes[] = {0, 2, 1};

	for (auto i = 0u; i < 3; ++i)
	{
		auto a = args[i];
		std::vector<NamedArgumentPtr> out;
		std::string::const_iterator begin = std::begin(a);
		std::string::const_iterator end = std::end(a);
		bool r = qi::phrase_parse(begin, end, m_grammar.construction_args, qi::space, out);

		EXPECT_TRUE(r) << "Should have matched " << a;
		EXPECT_EQ(begin, end);
		EXPECT_EQ(out.size(), expectedSizes[i]);
	}
}

TEST_F(GraphReaderGrammarTest, test_named_expression_arg)
{
	std::string args[] = {"a : \"abc\"", "b:123.0", "c:$<1+2;>$"};

	for (auto a : args)
	{
		NamedArgumentPtr out;
		std::string::const_iterator begin = std::begin(a);
		std::string::const_iterator end = std::end(a);
		bool r = qi::phrase_parse(begin, end, m_grammar.named_expression_arg, qi::space, out);

		EXPECT_TRUE(r) << "Should have matched " << a;
		EXPECT_EQ(begin, end);
	}
}

TEST_F(GraphReaderGrammarTest, test_named_expression_arg_construction_string)
{
	NamedArgumentPtr out;
	std::string a = "a : \"abc\"";
	std::string::const_iterator begin = std::begin(a);
	std::string::const_iterator end = std::end(a);
	qi::phrase_parse(begin, end, m_grammar.named_expression_arg, qi::space, out);

	ASSERT_NE(out, nullptr);
	EXPECT_EQ(out->GetName(), "a");
	EXPECT_EQ(out->GetArgumentType(), NamedArgument::ArgumentType::String);
	EXPECT_EQ(out->GetArgumentValue(), "abc");
}

TEST_F(GraphReaderGrammarTest, test_named_expression_arg_construction_float)
{
	NamedArgumentPtr out;
	std::string a = "a : 123";
	std::string::const_iterator begin = std::begin(a);
	std::string::const_iterator end = std::end(a);
	qi::phrase_parse(begin, end, m_grammar.named_expression_arg, qi::space, out);

	ASSERT_NE(out, nullptr);
	EXPECT_EQ(out->GetName(), "a");
	EXPECT_EQ(out->GetArgumentType(), NamedArgument::ArgumentType::Float);
	EXPECT_EQ(std::atof(out->GetArgumentValue().c_str()), 123.0f);
}

TEST_F(GraphReaderGrammarTest, test_named_expression_arg_construction_expression)
{
	NamedArgumentPtr out;
	std::string a = "a : $<1+1>$";
	std::string::const_iterator begin = std::begin(a);
	std::string::const_iterator end = std::end(a);
	qi::phrase_parse(begin, end, m_grammar.named_expression_arg, qi::space, out);

	ASSERT_NE(out, nullptr);
	EXPECT_EQ(out->GetName(), "a");
	EXPECT_EQ(out->GetArgumentType(), NamedArgument::ArgumentType::Expression);
	EXPECT_EQ(out->GetArgumentValue(), "1+1");
}

TEST_F(GraphReaderGrammarTest, test_execution_args)
{
	std::string args[] = {"", "a : \"abc\", b:123.0, c:$<a*b;>$", "b:123.0"};
	std::size_t expectedSizes[] = {0, 3, 1};

	for (auto i = 0u; i < 3; ++i)
	{
		auto a = args[i];
		std::vector<NamedArgumentPtr> out;
		std::string::const_iterator begin = std::begin(a);
		std::string::const_iterator end = std::end(a);
		bool r = qi::phrase_parse(begin, end, m_grammar.execution_args, qi::space, out);

		EXPECT_TRUE(r) << "Should have matched " << a;
		EXPECT_EQ(begin, end);
		EXPECT_EQ(out.size(), expectedSizes[i]);
	}
}

TEST_F(GraphReaderGrammarTest, test_node_definition)
{
	std::string def[] = {"n = Operation(a:1,b:2)", "n = Operation(a:1){}", "n = Operation(a:1){a:$<1+1;>$}"};

	for (auto a : def)
	{
		NodeDefinitionNodePtr out;
		std::string::const_iterator begin = std::begin(a);
		std::string::const_iterator end = std::end(a);
		bool r = qi::phrase_parse(begin, end, m_grammar.node_definition, qi::space, out);

		EXPECT_TRUE(r);
		EXPECT_EQ(begin, end) << "Should have matched " << a;
	}
}

TEST_F(GraphReaderGrammarTest, test_node_definition_construction)
{
	std::string a = "n = Operation(a:1,b:2){c:3}";
	NodeDefinitionNodePtr out;
	std::string::const_iterator begin = std::begin(a);
	std::string::const_iterator end = std::end(a);
	qi::phrase_parse(begin, end, m_grammar.node_definition, qi::space, out);

	ASSERT_NE(out, nullptr);
	EXPECT_EQ(out->GetNodeName(), "n");
	EXPECT_EQ(out->GetNodeType(), "Operation");
	EXPECT_EQ(out->GetConstructionArguments().size(), 2);
	EXPECT_EQ(out->GetExecutionArguments().size(), 1);
}

TEST_F(GraphReaderGrammarTest, test_node_links)
{
	std::string links[] = {"n1 -> n2", "n1->n2->n3"};

	for (auto a : links)
	{
		NodeLinkNodePtr out;
		std::string::const_iterator begin = std::begin(a);
		std::string::const_iterator end = std::end(a);
		bool r = qi::phrase_parse(begin, end, m_grammar.node_links, qi::space, out);

		EXPECT_TRUE(r);
		EXPECT_EQ(begin, end) << "Should have matched " << a;
	}
}

TEST_F(GraphReaderGrammarTest, test_node_links_construction)
{
	std::string a = "n1->n2->n3";
	NodeLinkNodePtr out;
	std::string::const_iterator begin = std::begin(a);
	std::string::const_iterator end = std::end(a);
	qi::phrase_parse(begin, end, m_grammar.node_links, qi::space, out);

	ASSERT_NE(out, nullptr);
	ASSERT_EQ(out->GetLinkedNodes().size(), 3);

	std::string expectedNodeNames[] = {"n1", "n2", "n3"};
	uint32_t i = 0;
	for (const auto &n : out->GetLinkedNodes())
	{
		EXPECT_EQ(n, expectedNodeNames[i++]);
	}
}
