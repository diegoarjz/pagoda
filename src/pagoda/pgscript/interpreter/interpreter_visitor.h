#pragma once

#include "../ir/ast_visitor.h"

#include <memory>
#include <stack>

namespace pagoda
{
namespace dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;
class Function;
using FunctionPtr = std::shared_ptr<Function>;
class DynamicValueTable;
class ICallable;
using ICallablePtr = std::shared_ptr<ICallable>;
}  // namespace dynamic

namespace ast
{
struct Float;
using FloatPtr = std::shared_ptr<Float>;
struct Integer;
using IntegerPtr = std::shared_ptr<Integer>;
struct String;
using StringPtr = std::shared_ptr<String>;
struct Identifier;
using IdentifierPtr = std::shared_ptr<Identifier>;
struct Boolean;
using BooleanPtr = std::shared_ptr<Boolean>;
struct Null;
using NullPtr = std::shared_ptr<Null>;
struct ArithmeticOp;
using ArithmeticOpPtr = std::shared_ptr<ArithmeticOp>;
struct Unary;
using UnaryPtr = std::shared_ptr<Unary>;
struct ComparisonOp;
using ComparisonOpPtr = std::shared_ptr<ComparisonOp>;
struct LogicOp;
using LogicOpPtr = std::shared_ptr<LogicOp>;
struct Assignment;
using AssignmentPtr = std::shared_ptr<Assignment>;
struct ExpressionStatement;
using ExpressionStatementPtr = std::shared_ptr<ExpressionStatement>;
struct IfStatement;
using IfStatementPtr = std::shared_ptr<IfStatement>;
struct Loop;
using LoopPtr = std::shared_ptr<Loop>;
struct VarDecl;
using VarDeclPtr = std::shared_ptr<VarDecl>;
struct StatementBlock;
using StatementBlockPtr = std::shared_ptr<StatementBlock>;
struct Call;
using CallPtr = std::shared_ptr<Call>;
struct FunctionDeclaration;
using FunctionDeclarationPtr = std::shared_ptr<FunctionDeclaration>;
class ClassDeclaration;
using ClassDeclarationPtr = std::shared_ptr<ClassDeclaration>;
class Program;
using ProgramPtr = std::shared_ptr<Program>;
}  // namespace ast

struct interpreter_visitor : public AstVisitor
{
	interpreter_visitor();
	~interpreter_visitor();

	void Visit(ast::AnonymousMethodPtr c) override;
	void Visit(ast::ArithmeticOpPtr op) override;
	void Visit(ast::AssignmentPtr a) override;
	void Visit(ast::BooleanPtr b) override;
	void Visit(ast::CallPtr c) override;
	void Visit(ast::ClassDeclarationPtr c) override;
	void Visit(ast::ComparisonOpPtr op) override;
	void Visit(ast::ExpressionStatementPtr e) override;
	void Visit(ast::FloatPtr n) override;
	void Visit(ast::FunctionDeclarationPtr func) override;
	void Visit(ast::GetExpressionPtr) override;
	void Visit(ast::IdentifierPtr i) override;
	void Visit(ast::IfStatementPtr i) override;
	void Visit(ast::IntegerPtr n) override;
	void Visit(ast::LogicOpPtr op) override;
	void Visit(ast::LoopPtr l) override;
	void Visit(ast::Nullptr n) override;
	void Visit(ast::ProgramPtr p) override;
	void Visit(ast::ReturnPtr r) override;
	void Visit(ast::SetExpressionPtr) override;
	void Visit(ast::StatementBlockPtr b) override;
	void Visit(ast::StringPtr s) override;
	void Visit(ast::UnaryPtr u) override;
	void Visit(ast::VarDeclPtr v) override;
	void Visit(ast::ParameterPtr p) override;

	void PushValue(const dynamic::DynamicValueBasePtr &v);
	dynamic::DynamicValueBasePtr PopValue();

	void EnterBlock();
	void ExitBlock(const std::shared_ptr<dynamic::DynamicValueTable> &previousSymbolTable);
	void EnterFunction(const dynamic::ICallablePtr &callable);
	void ExitFunction(const std::shared_ptr<dynamic::DynamicValueTable> &previousSymbolTable);

	std::shared_ptr<dynamic::DynamicValueTable> GetCurrentSymbolTable() { return m_symbolTable; }
	std::shared_ptr<dynamic::DynamicValueTable> GetGlobals() { return m_globals; }
	dynamic::DynamicValueBasePtr GetLastEvaluatedExpression() const { return m_lastValue; }

	private:
	std::stack<dynamic::DynamicValueBasePtr> m_values;
	std::shared_ptr<dynamic::DynamicValueTable> m_globals;
	std::shared_ptr<dynamic::DynamicValueTable> m_symbolTable;
	dynamic::DynamicValueBasePtr m_lastValue;
};
}  // namespace pagoda
