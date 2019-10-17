#pragma once

#include "function.h"

namespace sscript
{
template<class I>
class BuiltinMemberFunctionBase : public Function
{
public:
	virtual std::shared_ptr<BuiltinMemberFunctionBase> MakeCopy() = 0;

	void SetBoundInstance(std::shared_ptr<I> boundInstance) { m_boundInstance = boundInstance; }

protected:
	std::weak_ptr<I> m_boundInstance;
};

template<class I, typename FunctionType>
class BuiltinMemberFunction : public BuiltinMemberFunctionBase<I>
{
public:
	BuiltinMemberFunction(FunctionType function) : m_function(function) {}
	BuiltinMemberFunction(BuiltinMemberFunction& member)
	    : BuiltinMemberFunctionBase<I>(member), m_function(member.m_function)
	{
	}
	virtual ~BuiltinMemberFunction() {}

	void Call(interpreter_visitor* interpreter, const std::vector<BaseValuePtr>& args) override
	{
		return_from_member_function(*(BuiltinMemberFunctionBase<I>::m_boundInstance.lock()), m_function, args);
	}

	std::shared_ptr<BuiltinMemberFunctionBase<I>> MakeCopy() override
	{
		return std::make_shared<BuiltinMemberFunction<I, FunctionType>>(*this);
	}

private:
	FunctionType m_function;
};

}  // namespace sscript
