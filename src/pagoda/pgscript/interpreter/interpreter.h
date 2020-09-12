#pragma once

#include <memory>
#include <string>

namespace pagoda
{
namespace ast
{
class Program;
using ProgramPtr = std::shared_ptr<Program>;
}  // namespace ast

namespace dynamic
{
class DynamicValueBase;
using DynamicValueBasePtr = std::shared_ptr<DynamicValueBase>;

class DynamicValueTable;
}  // namespace dynamic

class Interpreter
{
	public:
	Interpreter();
	~Interpreter();

	bool Interpret(const ast::ProgramPtr &program);

	void PushExternalSymbols(std::shared_ptr<dynamic::DynamicValueTable> &externalSymbols);
	void PopExternalSymbols();

	dynamic::DynamicValueBasePtr GetLastEvaluatedExpression() const;

	static void SetStdOutStream(std::ostream *o);
	static std::ostream *GetStdOutStream();
	static void SetStdErrStream(std::ostream *o);
	static std::ostream *GetStdErrStream();

	private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pagoda
