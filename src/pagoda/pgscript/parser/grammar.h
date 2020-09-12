#pragma once

#include "../ir/ast.h"
#include "../ir/class_declaration.h"
#include "grammar_helpers.h"

#include <boost/phoenix/object/construct.hpp>
#include <boost/phoenix/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

using namespace boost::spirit::qi;

namespace pagoda
{
template<class Iterator>
struct grammar : boost::spirit::qi::grammar<Iterator, ast::ProgramPtr(), boost::spirit::qi::space_type>
{
	static auto kw(char const *keyword) -> boost::spirit::qi::rule<Iterator, void(), boost::spirit::qi::space_type>
	{
		using namespace boost::spirit;
		return lexeme[(qi::string(+keyword) >> !(qi::alnum | qi::char_('_')))];
	}

	static auto typed_kw(char const *keyword)
	  -> boost::spirit::qi::rule<Iterator, std::string(), boost::spirit::qi::space_type>
	{
		using namespace boost::spirit;
		return lexeme[(qi::string(keyword) >> !(qi::alnum | qi::char_('_')))[_val = qi::_1]];
	}

	grammar() : grammar::base_type(program)
	{
		using namespace boost::spirit;
		using boost::phoenix::bind;
		using boost::phoenix::construct;

		kw_true = kw("true");
		kw_false = kw("false");
		kw_null = kw("null");
		kw_var = kw("var");
		kw_if = kw("if");
		kw_else = kw("else");
		kw_while = kw("while");
		kw_for = kw("for");
		kw_or = typed_kw("or");
		kw_and = typed_kw("and");
		kw_function = kw("function");
		kw_return = kw("return");
		kw_class = kw("class");

		identifier = lexeme[(+(char_('_') | alpha) >> *(alnum | char_('_')))[_val = bind(make_identifier, qi::_1, qi::_2)]];
		number = Integer | Float;
		Float = float_[_val = bind(make_float, qi::_1)];
		Integer = int_[_val = bind(make_int, qi::_1)] >> !float_;
		string = lexeme[(('"' >> *(char_ - '"')) > '"')[_val = bind(make_string, qi::_1)]];

		/*
		 * program -> declaration* EOF
		 */
		program = (*declaration >> eoi)[_val = bind(make_program, qi::_1)];

		/*
		 * declaration -> var_decl | statement
		 */
		declaration = class_declaration | function_declaration | var_decl | statement;

		/*
		 * function_declaration -> "function" identifier "(" parameters? ")" statement_block
		 */
		function_declaration = (kw_function >> identifier >> '(' >> -parameters >> ')' >>
		                        statement_block)[_val = bind(make_function, qi::_1, qi::_2, qi::_3)];

		/*
		 * class_declaration -> "class" identifier "{" function_declaration* "}"
		 */
		class_declaration = (kw_class >> identifier >> '{' >> *function_declaration >>
		                     '}')[_val = bind(make_class_declaration, qi::_1, qi::_2)];

		/*
		 * parameter -> identifier ( ":" identifier )?
		 */
		parameter = identifier[_val = bind(make_parameter, qi::_1)] >>
		            -(':' >> identifier[_val = bind(add_parameter_type, _val, qi::_1)]);

		/*
		 * parameters -> identifier ( "," identifier )*
		 */
		parameters = parameter % ',';

		/*
		 * return_statement -> "return" expression? ';'
		 */
		return_statement = (kw_return >> -expression >> ';')[_val = bind(make_return_statement, qi::_1)];

		/*
		 * var_decl -> "var" IDENTIFIER ( "=" expression )? ";"
		 */
		var_decl = ("var" > identifier >> -('=' > expression) > ';')[_val = bind(make_var_decl, qi::_1, qi::_2)];

		/*
		 * statement -> expression_statement | if_statement | block
		 */
		statement = if_statement | while_loop | for_loop | return_statement | expression_statement | statement_block;

		/*
		 * statement_block -> "{" declaration* "}"
		 */
		statement_block =
		  qi::char_('{')[_val = bind(make_statement_block)] >> *(declaration[bind(add_statement, _val, qi::_1)]) >> '}';

		/*
		 * expression_statement -> expression ";"
		 */
		expression_statement = (expression[_val = bind(make_expression_statment, qi::_1)] > ';');

		/*
		 * if_statement -> "if" "(" expression ")" statement ("else" statement)?
		 */
		if_statement =
		  (qi::lit("if") >> '(' >> expression >> ')' >> statement)[_val = bind(make_if_statement, qi::_1, qi::_2)] >>
		  -(qi::lit("else") >> statement[_val = bind(add_else_statement, _val, qi::_1)]);

		/*
		 *  while_loop -> "while" "(" expression ")" statement
		 */
		while_loop =
		  (qi::lit("while") >> '(' >> expression >> ')' >> statement)[_val = bind(make_while_loop, qi::_1, qi::_2)];

		/*
		 * for_loop -> "for" "(" (var_decl | expression | ";")? expression? ";" expression? ")" statement
		 */
		for_loop = (qi::lit("for") >> '(' >> (var_decl | expression_statement | ';') >> -expression >> ';' >> -expression >>
		            ')' >> statement)[_val = bind(make_for_loop, qi::_1, qi::_2, qi::_3, qi::_4)];

		/*
		 * expression -> assignment;
		 */
		expression = assignment[_val = bind(make_expression, qi::_1)];

		/*
		 * assignment -> identifier "=" assignment | logic_or
		 */
		assignment =
		  (identifier >> '=' >> assignment)[_val = bind(make_assignment, qi::_1, qi::_2)] | logic_or[_val = qi::_1];

		/*
		 * logic_or -> logic_and ( "or" logic_and )*
		 */
		logic_or =
		  logic_and[_val = qi::_1] >> *((qi::string("or") >> logic_and)[_val = bind(make_logic_op, qi::_1, _val, qi::_2)]);

		/*
		 * logic_and -> equality ( "and" equality )*
		 */
		logic_and =
		  equality[_val = qi::_1] >> *((qi::string("and") >> equality)[_val = bind(make_logic_op, qi::_1, _val, qi::_2)]);

		/*
		 * equality -> comparison (("==" | "!=") comparison )*
		 */
		equality =
		  comparison[_val = qi::_1] >>
		  *(((qi::string("==") | qi::string("!=")) >> comparison)[_val = bind(make_comparison_op, qi::_1, _val, qi::_2)]);

		/*
		 * comparison -> addition ((">" | ">=" | "<" | "<=") addition)*
		 */
		comparison =
		  addition[_val = qi::_1] >> *(((qi::string(">=") | qi::string(">") | qi::string("<=") | qi::string("<")) >>
		                                addition)[_val = bind(make_comparison_op, qi::_1, _val, qi::_2)]);

		/*
		 * addition -> multiplication (("+"|"-") multiplication)*
		 */
		addition = multiplication[_val = qi::_1] >>
		           *(((char_('+') | char_('-')) >> multiplication)[_val = bind(make_arithmetic_op, qi::_1, _val, qi::_2)]);

		/*
		 * multiplication -> unary (("*"|"/") unary)*
		 */
		multiplication = unary[_val = qi::_1] >>
		                 *(((char_('*') | char_('/')) >> unary)[_val = bind(make_arithmetic_op, qi::_1, _val, qi::_2)]);

		/*
		 * unary -> (("!"|"-") unary) | call
		 */
		unary = ((char_('!') | char_('-')) >> unary)[_val = bind(make_unary_op, qi::_1, qi::_2)] | call[_val = qi::_1];

		/*
		 * call -> primary ( "(" arguments? ")" | ("." identifier ('=' assignment)?) | anonymous_method)*
		 */
		call = primary[_val = qi::_1] >> *('(' >> (-arguments)[_val = bind(make_call, _val, qi::_1)] >> ')' |
		                                   ('.' >> identifier[_val = bind(make_get_expression, _val, qi::_1)] >>
		                                    -('=' >> assignment[_val = bind(make_setter, qi::_val, qi::_1)])) |
		                                   statement_block[_val = bind(make_anonymous_method, qi::_val, qi::_1)]);

		/*
		 * arguments -> expression ( "," expression )*
		 */
		arguments = expression % ',';

		/*
		 * primary -> "true" | "false" | "null" | Float | string | identifier
		 *            | "(" expression ")"
		 */
		primary = kw_true[_val = bind(make_true)] | kw_false[_val = bind(make_false)] | kw_null[_val = bind(make_null)] |
		          (number[_val = qi::_1] | string[_val = qi::_1] | identifier[_val = qi::_1]) |
		          ('(' >> expression[_val = qi::_1] > ')');

		kw_true.name("kw_true");
		kw_false.name("kw_false");
		kw_null.name("kw_null");
		kw_var.name("kw_var");
		kw_if.name("kw_if");
		kw_else.name("kw_else");
		kw_while.name("kw_while");
		kw_for.name("kw_for");
		kw_function.name("kw_function");
		kw_return.name("kw_return");
		kw_class.name("kw_class");
		kw_or.name("kw_or");
		kw_and.name("kw_and");
		identifier.name("identifier");
		number.name("number");
		Float.name("Float");
		Integer.name("Integer");
		string.name("string");
		program.name("program");
		declaration.name("declaration");
		var_decl.name("var_decl");
		statement.name("statement");
		statement_block.name("statement_block");
		if_statement.name("if_statement");
		while_loop.name("while_loop");
		for_loop.name("for_loop");
		expression.name("expression");
		expression_statement.name("expression_statement");
		assignment.name("assignment");
		logic_or.name("logic_or");
		logic_and.name("logic_and");
		equality.name("equality");
		comparison.name("comparison");
		addition.name("addition");
		multiplication.name("multiplication");
		unary.name("unary");
		call.name("call");
		arguments.name("arguments");
		function_declaration.name("function_declaration");
		parameters.name("parameters");
		return_statement.name("return_statement");
		class_declaration.name("class_declaration");
		primary.name("primary");
	}

	rule<Iterator, void(), space_type> kw_true;
	rule<Iterator, void(), space_type> kw_false;
	rule<Iterator, void(), space_type> kw_null;
	rule<Iterator, void(), space_type> kw_var;
	rule<Iterator, void(), space_type> kw_if;
	rule<Iterator, void(), space_type> kw_else;
	rule<Iterator, void(), space_type> kw_while;
	rule<Iterator, void(), space_type> kw_for;
	rule<Iterator, void(), space_type> kw_function;
	rule<Iterator, void(), space_type> kw_return;
	rule<Iterator, void(), space_type> kw_class;
	rule<Iterator, std::string(), space_type> kw_or;
	rule<Iterator, std::string(), space_type> kw_and;

	rule<Iterator, ast::IdentifierPtr(), space_type> identifier;
	rule<Iterator, ast::ExpressionPtr(), space_type> number;
	rule<Iterator, ast::FloatPtr(), space_type> Float;
	rule<Iterator, ast::IntegerPtr(), space_type> Integer;
	rule<Iterator, ast::StringPtr(), space_type> string;

	rule<Iterator, ast::ProgramPtr(), space_type> program;
	rule<Iterator, ast::StatementPtr(), space_type> declaration;
	rule<Iterator, ast::VarDeclPtr(), space_type> var_decl;
	rule<Iterator, ast::StatementPtr(), space_type> statement;
	rule<Iterator, ast::StatementBlockPtr(), space_type> statement_block;
	rule<Iterator, ast::IfStatementPtr(), space_type> if_statement;
	rule<Iterator, ast::LoopPtr(), space_type> while_loop;
	rule<Iterator, ast::StatementPtr(), space_type> for_loop;
	rule<Iterator, ast::ExpressionPtr(), space_type> expression;
	rule<Iterator, ast::ExpressionStatementPtr(), space_type> expression_statement;
	rule<Iterator, ast::ExpressionPtr(), space_type> assignment;
	rule<Iterator, ast::ExpressionPtr(), space_type> logic_or;
	rule<Iterator, ast::ExpressionPtr(), space_type> logic_and;
	rule<Iterator, ast::ExpressionPtr(), space_type> equality;
	rule<Iterator, ast::ExpressionPtr(), space_type> comparison;
	rule<Iterator, ast::ExpressionPtr(), space_type> addition;
	rule<Iterator, ast::ExpressionPtr(), space_type> multiplication;
	rule<Iterator, ast::ExpressionPtr(), space_type> unary;
	rule<Iterator, ast::ExpressionPtr(), space_type> call;
	rule<Iterator, std::vector<ast::ExpressionPtr>(), space_type> arguments;
	rule<Iterator, ast::FunctionDeclarationPtr(), space_type> function_declaration;
	rule<Iterator, ast::ParameterPtr(), space_type> parameter;
	rule<Iterator, std::vector<ast::ParameterPtr>(), space_type> parameters;
	rule<Iterator, ast::ReturnPtr(), space_type> return_statement;
	rule<Iterator, ast::ClassDeclarationPtr(), space_type> class_declaration;
	rule<Iterator, ast::ExpressionPtr(), space_type> primary;
};
}  // namespace pagoda
