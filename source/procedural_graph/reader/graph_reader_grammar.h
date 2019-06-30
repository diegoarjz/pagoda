#ifndef SELECTOR_PROCEDURAL_GRAPH_GRAPH_FORMAT_GRAMMAR_H_
#define SELECTOR_PROCEDURAL_GRAPH_GRAPH_FORMAT_GRAMMAR_H_

#include "graph_definition_node.h"

#include <boost/spirit/include/qi.hpp>

namespace selector
{
template<class Iterator>
struct GraphReaderGrammar
    : boost::spirit::qi::grammar<Iterator, GraphDefinitionNodePtr(), boost::spirit::qi::space_type>
{
	template<typename... Args>
	using Rule_t = boost::spirit::qi::rule<Args..., boost::spirit::qi::space_type>;

	GraphReaderGrammar() : GraphReaderGrammar::base_type(graph_definition)
	{
		/*
		 * graph_definition -> (node_definition | node_links)*
		 * node_definition -> identifier = identifier
		 *                      "(" construction_args ")"
		 *                      ("{" execution_args "}")?
		 * identifier -> ("_" | alpha) (alnum | "_")*
		 * literal -> quoted_string | float
		 * expression -> "$<" expression_body ">$"
		 * expression_body -> .* \ ">$"
		 * construction_args -> (named_simple_arg ("," named_simple_arg)*)?
		 * named_simple_arg -> identifier ":" literal
		 * execution_args -> (named_expression_arg ("," named_expression_arg)*)?
		 * named_expression_arg -> identifier ":" ( expression | literal )
		 * node_links -> identifier "->" identifier ("->" identifier)*
		 */

		using namespace boost::spirit;
		using namespace boost::spirit::qi;

		/*
		 * identifier -> ("_" | alpha) (alnum | "_")*
		 */
		identifier %= (char_('_') | alpha) >> *(alnum | char_('_'));
		quoted_string %= ('"' > lexeme[*(char_ - char_('"'))] >> '"');

		/*
		 * literal -> quoted_string | float
		 */
		literal = quoted_string | float_;

		/*
		 * expression -> "$<" expression_body ">$"
		 */
		expression = "$<" >> expression_body >> ">$";

		/*
		 * expression_body = .* \ ">$"
		 */
		expression_body = lexeme[*(char_ - ">$")];

		/*
		 * named_simple_arg -> identifier ":" literal
		 */
		named_simple_arg = identifier >> ":" >> literal;

		/*
		 * construction_args -> (named_simple_arg ("," named_simple_arg)*)?
		 */
		construction_args = -(named_simple_arg % ',');

		/*
		 * named_expression_arg -> identifier ":" ( expression | literal )
		 */
		named_expression_arg = identifier >> ":" >> (expression | literal);

		/*
		 * execution_args -> (named_expression_arg ("," named_expression_arg)*)?
		 */
		execution_args = -(named_expression_arg % ',');

		/*
		 * node_definition -> identifier = identifier
		 *                      "(" construction_args ")"
		 *                      ("{" execution_args "}")?
		 */
		node_definition =
		    identifier >> '=' >> identifier >> '(' >> construction_args >> ')' >> -('{' >> execution_args >> '}');

		/*
		 * node_links -> identifier "->" identifier ("->" identifier)*
		 */
		node_links = identifier >> "->" >> (identifier % "->");

		/*
		 * graph_definition -> (node_definition | node_links)*
		 */
		graph_definition = *(node_definition | node_links);
	}

	Rule_t<Iterator, std::string()> identifier;
	Rule_t<Iterator, std::string()> quoted_string;
	Rule_t<Iterator, boost::variant<std::string, float>()> literal;
	Rule_t<Iterator, std::string()> expression;
	Rule_t<Iterator, std::string()> expression_body;
	Rule_t<Iterator, boost::spirit::unused_type> named_simple_arg;
	Rule_t<Iterator, boost::spirit::unused_type> construction_args;
	Rule_t<Iterator, boost::spirit::unused_type> named_expression_arg;
	Rule_t<Iterator, boost::spirit::unused_type> execution_args;
	Rule_t<Iterator, boost::spirit::unused_type> node_definition;
	Rule_t<Iterator, boost::spirit::unused_type> node_links;
	Rule_t<Iterator, GraphDefinitionNodePtr()> graph_definition;
};

}  // namespace selector

#endif
