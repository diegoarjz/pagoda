#pragma once

#include "cast_to_base_value.h"
#include "functional.h"

#include "../value/base_value.h"

#include <functional>

namespace sscript
{
template<typename R, typename... Args>
struct return_from_function_helper
{
	static void call_and_return(std::function<R(Args...)> function, const std::vector<BaseValuePtr>& args)
	{
		throw static_cast<BaseValuePtr>(cast_to_base_value<R>::cast(call_function(function, args)));
	}
};

template<typename... Args>
struct return_from_function_helper<void, Args...>
{
	static void call_and_return(std::function<void(Args...)> function, const std::vector<BaseValuePtr>& args)
	{
		call_function(function, args);
	}
};

template<typename R, typename... Args>
void return_from_function(std::function<R(Args...)> function, const std::vector<BaseValuePtr>& args)
{
	return_from_function_helper<R, Args...>::call_and_return(function, args);
}

template<typename R, class C, typename... Args>
struct return_from_member_function_helper
{
	static void call_and_return(C& instance, std::function<R(C, Args...)> member, const std::vector<BaseValuePtr>& args)
	{
		throw static_cast<BaseValuePtr>(cast_to_base_value<R>::cast(call_member_function(instance, member, args)));
	}
};

template<class C, typename... Args>
struct return_from_member_function_helper<void, C, Args...>
{
	static void call_and_return(C& instance, std::function<void(C, Args...)> member,
	                            const std::vector<BaseValuePtr>& args)
	{
		call_member_function(instance, member, args);
	}
};

template<typename R, class C, typename... Args>
void return_from_member_function(C& instance, std::function<R(C, Args...)> member,
                                 const std::vector<BaseValuePtr>& args)
{
	return_from_member_function_helper<R, C, Args...>::call_and_return(instance, member, args);
}

}  // namespace sscript
