#pragma once

#include "anonymous_method.h"
#include "ast.h"
#include "class_declaration.h"
#include "get_expression.h"
#include "parameter.h"
#include "set_expression.h"

namespace sscript
{
class AstVisitor
{
public:
	virtual void Visit(ast::AnonymousMethodPtr) = 0;
	virtual void Visit(ast::ArithmeticOpPtr) = 0;
	virtual void Visit(ast::AssignmentPtr) = 0;
	virtual void Visit(ast::BooleanPtr) = 0;
	virtual void Visit(ast::CallPtr) = 0;
	virtual void Visit(ast::ClassDeclarationPtr) = 0;
	virtual void Visit(ast::ComparisonOpPtr) = 0;
	virtual void Visit(ast::ExpressionStatementPtr) = 0;
	virtual void Visit(ast::FloatPtr) = 0;
	virtual void Visit(ast::FunctionDeclarationPtr) = 0;
	virtual void Visit(ast::GetExpressionPtr) = 0;
	virtual void Visit(ast::IdentifierPtr) = 0;
	virtual void Visit(ast::IfStatementPtr) = 0;
	virtual void Visit(ast::IntegerPtr) = 0;
	virtual void Visit(ast::LogicOpPtr) = 0;
	virtual void Visit(ast::LoopPtr) = 0;
	virtual void Visit(ast::Nullptr) = 0;
	virtual void Visit(ast::ProgramPtr) = 0;
	virtual void Visit(ast::ReturnPtr) = 0;
	virtual void Visit(ast::SetExpressionPtr) = 0;
	virtual void Visit(ast::StatementBlockPtr) = 0;
	virtual void Visit(ast::StringPtr) = 0;
	virtual void Visit(ast::UnaryPtr) = 0;
	virtual void Visit(ast::VarDeclPtr) = 0;
	virtual void Visit(ast::ParameterPtr) = 0;
};
}  // namespace sscript
