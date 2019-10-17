#pragma once

#include "../intermediate/anonymous_method.h"
#include "../intermediate/ast.h"
#include "../intermediate/class_declaration.h"
#include "../intermediate/get_expression.h"
#include "../intermediate/set_expression.h"
#include "../intermediate/parameter.h"

namespace sscript
{
ast::IdentifierPtr make_identifier(const std::vector<char> &identifier);

ast::FloatPtr make_float(const float &f);

ast::IntegerPtr make_int(const int &i);

ast::StringPtr make_string(const std::vector<char> &s);

ast::BooleanPtr make_true();
ast::BooleanPtr make_false();
ast::Nullptr make_null();

ast::UnaryPtr make_unary_op(char op, const ast::ExpressionPtr &rhs);

ast::ArithmeticOpPtr make_arithmetic_op(char op, const ast::ExpressionPtr &lhs, const ast::ExpressionPtr &rhs);

ast::ComparisonOpPtr make_comparison_op(const std::string &op, const ast::ExpressionPtr &lhs,
                                        const ast::ExpressionPtr &rhs);

ast::LogicOpPtr make_logic_op(const std::string &op, const ast::ExpressionPtr &lhs, const ast::ExpressionPtr &rhs);

ast::ExpressionPtr make_expression(const ast::ExpressionPtr &expression);

ast::ExpressionStatementPtr make_expression_statment(const ast::ExpressionPtr &expression);

ast::AssignmentPtr make_assignment(const ast::IdentifierPtr &identifier, const ast::ExpressionPtr &rhs);

ast::LoopPtr make_while_loop(const ast::ExpressionPtr &condition, const ast::StatementPtr body);

ast::IfStatementPtr make_if_statement(ast::ExpressionPtr &condition, ast::StatementPtr &trueStatement);

ast::IfStatementPtr &add_else_statement(ast::IfStatementPtr &ifStatement, ast::StatementPtr &elseStatement);

ast::StatementBlockPtr make_statement_block();

void add_statement(ast::StatementBlockPtr &block, ast::StatementPtr &statement);

ast::VarDeclPtr make_var_decl(ast::IdentifierPtr &identifier, boost::optional<ast::ExpressionPtr> &initializer);

ast::ProgramPtr make_program(const std::vector<ast::StatementPtr> &statements);

ast::CallPtr make_call(const ast::ExpressionPtr &callee, boost::optional<std::vector<ast::ExpressionPtr>> args);
ast::GetExpressionPtr make_get_expression(const ast::ExpressionPtr &lhs, const ast::IdentifierPtr &identifier);
ast::SetExpressionPtr make_setter(const ast::ExpressionPtr &lhs, const ast::ExpressionPtr &rhs);

ast::FunctionDeclarationPtr make_function(ast::IdentifierPtr identifier,
                                          boost::optional<std::vector<ast::ParameterPtr>> &parameters,
                                          const ast::StatementBlockPtr body);

ast::ParameterPtr make_parameter(ast::IdentifierPtr identifier);
ast::ParameterPtr add_parameter_type(ast::ParameterPtr parameter, ast::IdentifierPtr parameterType);

ast::ReturnPtr make_return_statement(boost::optional<ast::ExpressionPtr> return_expression);
ast::AnonymousMethodPtr make_anonymous_method(const ast::ExpressionPtr &instance, const ast::StatementBlockPtr &body);

ast::ClassDeclarationPtr make_class_declaration(ast::IdentifierPtr identifier,
                                                std::vector<ast::FunctionDeclarationPtr> methods);

ast::StatementPtr make_for_loop(
    const boost::optional<boost::variant<ast::VarDeclPtr, ast::ExpressionStatementPtr>> &init,
    const boost::optional<ast::ExpressionPtr> &condition, const boost::optional<ast::ExpressionPtr> &increment,
    const ast::StatementPtr body);

}  // namespace sscript
