#pragma once

#include "icallable_body.h"

#include "binding/return_from_function.h"

namespace pagoda::dynamic
{
/**
 * Implements a wrapper for a c++ free function \c ICallableBody.
 *
 * The arguments and return value types must be of \c DynamicValueBase.
 */
template<typename FunctionType>
struct FreeFunctionCallableBody : public ICallableBody
{
public:
	/**
	 * Constructs the \c FreeFunctionCallableBody with the function in \p function.
	 */
	FreeFunctionCallableBody(FunctionType function) : m_function(function) {}
	virtual ~FreeFunctionCallableBody() {}

	/**
	 * Calls the wrapped free function with the arguments in \p args.
	 * This will throw the wrapped free function's return value and it should be catch'ed by the callee.
	 */
	void Call(const std::vector<DynamicValueBasePtr>& args) override { return_from_function(m_function, args); }

	void SetClosure(const std::shared_ptr<DynamicValueTable>& closure) override { m_closure = closure; }
	const std::shared_ptr<DynamicValueTable>& GetClosure() const override { return m_closure; }

private:
	FunctionType m_function;
	std::shared_ptr<DynamicValueTable> m_closure;
};
}  // namespace pagoda::dynamic
