#pragma once

#include <string>
#include <vector>

namespace sscript
{
namespace ast
{
struct StatementBlock;
using StatementBlockPtr = std::shared_ptr<StatementBlock>;
class Parameter;
using ParameterPtr = std::shared_ptr<Parameter>;
}  // namespace ast
class SymbolTable;
struct interpreter_visitor;
struct BaseValue;
using BaseValuePtr = std::shared_ptr<BaseValue>;

class Callable
{
public:
	virtual void SetCallableName(const std::string&) = 0;
	virtual const std::string& GetCallableName() const = 0;
	virtual void SetArity(const std::size_t&) = 0;
	virtual const std::size_t& GetArity() const = 0;
	virtual bool IsVariadic() const = 0;
	virtual void SetVariadic(bool) = 0;
	virtual void SetParameters(const std::vector<ast::ParameterPtr>&) = 0;
	virtual const std::vector<ast::ParameterPtr>& GetParameters() const = 0;
	virtual void SetClosure(const std::shared_ptr<SymbolTable>&) = 0;
	virtual const std::shared_ptr<SymbolTable>& GetClosure() const = 0;
	virtual void SetCallableBody(ast::StatementBlockPtr) = 0;

	virtual void Call(interpreter_visitor*, const std::vector<BaseValuePtr>&) = 0;
};

using CallablePtr = std::shared_ptr<Callable>;
}  // namespace sscript
