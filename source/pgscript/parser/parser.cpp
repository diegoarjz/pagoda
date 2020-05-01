#include "parser.h"

#include "grammar.h"

#include "../intermediate/ast.h"

#include <boost/spirit/include/support_line_pos_iterator.hpp>

namespace pagoda
{
Parser::Parser() {}

ast::ProgramPtr Parser::Parse(const std::string &source)
{
	using parser_iterator = boost::spirit::line_pos_iterator<std::string::const_iterator>;
	parser_iterator begin(std::cbegin(source));
	parser_iterator end(std::cend(source));

	grammar<parser_iterator> parser;
	ast::ProgramPtr program;

	try
	{
		boost::spirit::qi::phrase_parse(begin, end, parser, boost::spirit::qi::space, program);
	}
	catch (boost::spirit::qi::expectation_failure<parser_iterator> const &x)
	{
		std::cout << "Expected: " << x.what_;
		std::cout << " found: " << std::string(x.first, x.last);
		std::cout << " (line " << boost::spirit::get_line(x.first) << ")" << std::endl;
		auto range = boost::spirit::get_current_line(x.first, x.first, x.last);
		std::string line(range.begin(), range.end());
		std::cout << line << std::endl;
	}

	return program;
}
}  // namespace pagoda
