#pragma once

#include "../dynamic_value_base.h"
#include "call_free_function.h"

#include <functional>

namespace selector
{
/**
 * Calls the free function and throws its return value which must be a \c DynamicValueBasePtr.
 */
template<typename R, typename... Args>
struct return_from_function_helper
{
	static void call_and_return(std::function<R(Args...)> function, const std::vector<DynamicValueBasePtr>& args)
	{
		throw static_cast<DynamicValueBasePtr>(call_free_function(function, args));
	}
};

/**
 * Calls the free function whose return type is void.
 */
template<typename... Args>
struct return_from_function_helper<void, Args...>
{
	static void call_and_return(std::function<void(Args...)> function, const std::vector<DynamicValueBasePtr>& args)
	{
		call_free_function(function, args);
	}
};

/**
 * Implements the return policy for calling a free function.
 *
 * Instances of \c DynamicValueBase are thrown and must be caught by the callee.
 */
template<typename R, typename... Args>
void return_from_function(std::function<R(Args...)> function, const std::vector<DynamicValueBasePtr>& args)
{
	return_from_function_helper<R, Args...>::call_and_return(function, args);
}

}  // namespace selector
