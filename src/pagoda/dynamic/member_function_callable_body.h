#pragma once

#include "icallable_body.h"

#include "binding/return_from_member_function.h"

namespace pagoda::dynamic
{
/**
 * Implements a wrapper for a c++ member function \c ICallableBody.
 *
 * The arguments and return value types must be of \c DynamicValueBase.
 */
template<class InstanceType, typename FunctionType>
struct MemberFunctionCallableBody : public ICallableBody
{
public:
	/**
	 * Constructs the \c MemberFunctionCallableBody with the function in \p function.
	 */
	MemberFunctionCallableBody(InstanceType &instance, FunctionType function)
	    : m_instance(instance), m_function(function)
	{
	}
	virtual ~MemberFunctionCallableBody() {}

	/**
	 * Calls the wrapped member function with the arguments in \p args.
	 * This will throw the wrapped member function's return value and it should be catch'ed by the callee.
	 */
	void Call(const std::vector<DynamicValueBasePtr> &args) override
	{
		return_from_member_function(m_instance, m_function, args);
	}

	void SetClosure(const std::shared_ptr<DynamicValueTable> &closure) override { m_closure = closure; }
	const std::shared_ptr<DynamicValueTable> &GetClosure() const override { return m_closure; }

private:
	InstanceType &m_instance;
	FunctionType m_function;
	std::shared_ptr<DynamicValueTable> m_closure;
};
}  // namespace pagoda::dynamic
