#pragma once

#include <memory>
#include <string>

namespace sscript
{
namespace ast
{
class Program;
using ProgramPtr = std::shared_ptr<Program>;
}  // namespace ast

struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;

class SymbolTable;

class Interpreter
{
public:
	Interpreter();
	~Interpreter();

	bool Interpret(const ast::ProgramPtr &program);

	void PushExternalSymbols(std::shared_ptr<SymbolTable> &externalSymbols);
	void PopExternalSymbols();

	BaseValuePtr GetLastEvaluatedExpression() const;

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace sscript
