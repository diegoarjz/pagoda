#pragma once

#include "call_member_function.h"

namespace selector
{
/**
 * Calls the member function and throws its return value which must be a \c DynamicValueBasePtr.
 */
template<typename R, class C, typename... Args>
struct return_from_member_function_helper
{
	static void call_and_return(C& instance, std::function<R(C*, Args...)> member,
	                            const std::vector<DynamicValueBasePtr>& args)
	{
		throw static_cast<DynamicValueBasePtr>(call_member_function(instance, member, args));
	}
};

/**
 * Calls the member function whose return type is void.
 */
template<class C, typename... Args>
struct return_from_member_function_helper<void, C, Args...>
{
	static void call_and_return(C& instance, std::function<void(C*, Args...)> member,
	                            const std::vector<DynamicValueBasePtr>& args)
	{
		call_member_function(instance, member, args);
	}
};

/**
 * Implements the return policy for calling a member function \p member on the object reference in \p instance with the
 * args given in \p args.
 *
 * Instances of \c DynamicValueBase are thrown and must be caught by the callee.
 */
template<typename R, class C, typename... Args>
void return_from_member_function(C& instance, std::function<R(C*, Args...)> member,
                                 const std::vector<DynamicValueBasePtr>& args)
{
	return_from_member_function_helper<R, C, Args...>::call_and_return(instance, member, args);
}
}  // namespace selector
