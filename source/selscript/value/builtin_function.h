#pragma once

#include "../binding/return_from_function.h"
#include "function.h"

namespace sscript
{
template<typename FunctionType>
struct BuiltinFunction : public Function
{
public:
	BuiltinFunction(FunctionType function) : m_function(function) {}
	virtual ~BuiltinFunction() {}

	void Call(interpreter_visitor* interpreter, const std::vector<BaseValuePtr>& args)
	{
		return_from_function(m_function, args);
	}

private:
	FunctionType m_function;
};
}  // namespace sscript
