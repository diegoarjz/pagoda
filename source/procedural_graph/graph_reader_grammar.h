#ifndef SELECTOR_PROCEDURAL_GRAPH_GRAPH_READER_GRAMMAR_H_
#define SELECTOR_PROCEDURAL_GRAPH_GRAPH_READER_GRAMMAR_H_

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

#include <string>
#include <vector>

using namespace boost::spirit;

namespace selector
{
namespace ast
{
struct binary_op;
struct node_definition;

struct variable
{
	std::string m_name;

	variable() = default;
};

using literal = boost::variant<float, std::string>;
using expression =
    boost::variant<literal, variable, boost::recursive_wrapper<binary_op>, boost::recursive_wrapper<node_definition>>;

struct binary_op
{
	char m_operation;
	expression m_lhs;
	expression m_rhs;

	binary_op(char op, const expression &lhs, const expression &rhs) : m_operation(op), m_lhs(lhs), m_rhs(rhs) {}
};

struct assignment
{
	variable m_lhs;
	expression m_rhs;

	assignment() = default;
	assignment(const variable &v, const expression &e) : m_lhs(v), m_rhs(e) {}
};

struct named_arg
{
	std::string m_name;
	expression m_value;

	named_arg() = default;
	named_arg(const std::string &name, const expression &e) : m_name(name), m_value(e) {}
};

using named_arg_list = std::vector<named_arg>;

struct node_definition
{
	std::string m_nodeType;
	named_arg_list m_constructionArguments;
	named_arg_list m_nodeParameters;

	node_definition() = default;
	node_definition(const std::string type, const named_arg_list &construcionArgs, const named_arg_list &parameters)
	    : m_nodeType(type), m_constructionArguments(construcionArgs), m_nodeParameters(parameters)
	{
	}
};

using node_links = std::vector<variable>;

using statement = boost::variant<assignment, node_links>;

struct graph_definition
{
	std::vector<statement> m_statements;
};

};  // namespace ast
}  // namespace selector

// clang-format off
BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::variable,
    (std::string, m_name)
)

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::binary_op,
    (char, m_operation),
    (selector::ast::expression, m_lhs),
    (selector::ast::expression, m_rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::assignment,
    (selector::ast::variable, m_lhs),
    (selector::ast::expression, m_rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::node_definition,
    (std::string, m_nodeType),
    (selector::ast::named_arg_list, m_constructionArguments),
    (selector::ast::named_arg_list, m_nodeParameters)
)

BOOST_FUSION_ADAPT_STRUCT(
    selector::ast::graph_definition,
    (std::vector<selector::ast::statement>, m_statements)
)
// clang-format on

namespace selector
{
template<class Iterator>
struct GraphReaderGrammar : boost::spirit::qi::grammar<Iterator, ast::graph_definition(), boost::spirit::qi::space_type>
{
	GraphReaderGrammar() : GraphReaderGrammar::base_type(graph_definition)
	{
		using namespace boost::spirit;
		using namespace boost::spirit::qi;
		using namespace boost::phoenix;

		identifier %= (char_('_') | alpha) >> *(alnum | char_('_'));
		quoted_string %= ('"' > lexeme[*(char_ - char_('"'))] >> '"');
		literal = float_ | quoted_string;

		variable = identifier[at_c<0>(_val) = qi::_1];

		expression =
		    term[_val = qi::_1] >> *((char_("+-") >> term)[_val = construct<ast::binary_op>(qi::_1, _val, qi::_2)]);

		term =
		    factor[_val = qi::_1] >> *((char_("*/") >> factor)[_val = construct<ast::binary_op>(qi::_1, _val, qi::_2)]);

		factor = literal | node_definition | variable | ('(' >> expression >> ')');

		assignment %= variable >> '=' >> expression;

		named_arg = (identifier >> ':' >> expression)[_val = construct<ast::named_arg>(qi::_1, qi::_2)];
		named_arg_list %= -(named_arg % ',');

		node_definition %= identifier >> '(' >> named_arg_list >> ')' >> -('{' >> named_arg_list >> '}');

		node_links = variable[push_back(_val, qi::_1)] >> "->" >> variable[push_back(_val, qi::_1)] >>
		             *("->" >> variable[push_back(_val, qi::_1)]) >> ';';

		statement %= assignment | node_links;

		graph_definition = *(statement[push_back(at_c<0>(_val), qi::_1)]);
	}

	qi::rule<Iterator, std::string(), qi::space_type> identifier;
	qi::rule<Iterator, std::string(), qi::space_type> quoted_string;
	qi::rule<Iterator, ast::literal(), qi::space_type> literal;
	qi::rule<Iterator, ast::variable(), qi::space_type> variable;
	qi::rule<Iterator, ast::expression(), qi::space_type> expression;
	qi::rule<Iterator, ast::expression(), qi::space_type> term;
	qi::rule<Iterator, ast::expression(), qi::space_type> factor;
	qi::rule<Iterator, ast::assignment(), qi::space_type> assignment;
	qi::rule<Iterator, ast::named_arg(), qi::space_type> named_arg;
	qi::rule<Iterator, ast::named_arg_list(), qi::space_type> named_arg_list;
	qi::rule<Iterator, ast::node_definition(), qi::space_type> node_definition;
	qi::rule<Iterator, ast::node_links(), qi::space_type> node_links;
	qi::rule<Iterator, ast::statement(), qi::space_type> statement;
	qi::rule<Iterator, ast::graph_definition(), qi::space_type> graph_definition;
};

}  // namespace selector

#endif
