#ifndef SELECTOR_PROCEDURAL_GRAPH_GRAPH_READER_GRAMMAR_H_
#define SELECTOR_PROCEDURAL_GRAPH_GRAPH_READER_GRAMMAR_H_

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>
#include <vector>

using namespace boost::spirit;

namespace selector
{
namespace ast
{
struct literal
{
	using types = boost::variant<int, float, std::string>;
	types m_value;
};

struct variable
{
	std::string m_name;
};

struct parameter_value
{
	using types = boost::variant<literal, variable>;
	types m_value;
};

struct parameter
{
	std::string m_name;
	parameter_value m_value;
};

struct node_definition
{
	std::string m_name;
	std::string m_type;
	std::vector<parameter> m_nodeParameters;
};

struct instruction
{
	using types = boost::variant<node_definition, std::vector<std::string>>;
	types m_instruction;
};

struct graph_definition
{
	std::vector<instruction> m_instructions;
};

};  // namespace ast
}  // namespace selector

// clang-format off
BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::literal,
    (selector::ast::literal::types, m_value))

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::variable,
    (std::string, m_name))

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::parameter_value,
    (selector::ast::parameter_value::types, m_value))

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::parameter,
    (std::string, m_name),
    (selector::ast::parameter_value, m_value))

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::node_definition,
    (std::string, m_name),
    (std::string, m_type),
    (std::vector<selector::ast::parameter>, m_nodeParameters))

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::instruction,
    (selector::ast::instruction::types, m_instruction))


BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::graph_definition,
    (std::vector<selector::ast::instruction>, m_instructions))

// clang-format on

namespace selector
{
template<class Iterator>
struct GraphReaderGrammar
    : boost::spirit::qi::grammar<Iterator, std::vector<ast::instruction>(), boost::spirit::qi::space_type>
{
	GraphReaderGrammar() : GraphReaderGrammar::base_type(start)
	{
		identifier %= (qi::alpha | qi::char_('_')) >> *(qi::alnum | qi::char_('_'));

		quoted_string = '"' >> *(qi::char_ - '"') >> '"';

		literal %= float_ | int_ | quoted_string;

		variable %= identifier;

		parameter_value %= literal | variable;

		parameter %= identifier >> ':' >> parameter_value;

		parameter_list %= qi::lit('{') >> -(parameter % ',') >> qi::lit('}');

		node_definition %= identifier >> '=' >> identifier >> parameter_list;

		node_links %= identifier >> qi::lit("->") >> identifier >> +(qi::lit("->") >> identifier);

		instruction %= node_definition | node_links;

		start %= *instruction;
	}

	qi::rule<Iterator, std::string(), qi::space_type> identifier;
	qi::rule<Iterator, std::string(), qi::space_type> quoted_string;
	qi::rule<Iterator, ast::literal(), qi::space_type> literal;
	qi::rule<Iterator, ast::variable(), qi::space_type> variable;
	qi::rule<Iterator, ast::parameter_value(), qi::space_type> parameter_value;
	qi::rule<Iterator, ast::parameter(), qi::space_type> parameter;
	qi::rule<Iterator, std::vector<ast::parameter>(), qi::space_type> parameter_list;
	qi::rule<Iterator, ast::node_definition(), qi::space_type> node_definition;
	qi::rule<Iterator, std::vector<std::string>, qi::space_type> node_links;
	qi::rule<Iterator, ast::instruction(), qi::space_type> instruction;
	qi::rule<Iterator, std::vector<ast::instruction>(), qi::space_type> start;
};

}  // namespace selector

#endif
