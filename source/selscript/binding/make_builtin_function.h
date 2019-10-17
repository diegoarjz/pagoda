#pragma once

#include "../value/builtin_function.h"

namespace sscript
{
template<typename R, typename... Args>
struct make_builtin_function_helper
{
	static std::shared_ptr<BuiltinFunction<std::function<R(Args...)>>> make_builtin_function(
	    const std::string& functionName, std::function<R(Args...)> function)
	{
		auto callable = std::make_shared<BuiltinFunction<std::function<R(Args...)>>>(function);
		callable->SetArity(sizeof...(Args));
		callable->SetCallableName(functionName);

		return callable;
	}
};

template<typename R>
struct make_builtin_function_helper<R, const std::vector<BaseValuePtr>&>
{
	static std::shared_ptr<BuiltinFunction<std::function<R(const std::vector<BaseValuePtr>&)>>> make_builtin_function(
	    const std::string& functionName, std::function<R(const std::vector<BaseValuePtr>&)> function)
	{
		auto callable = std::make_shared<BuiltinFunction<std::function<R(const std::vector<BaseValuePtr>&)>>>(function);
		callable->SetCallableName(functionName);
		callable->SetVariadic(true);

		return callable;
	}
};

template<typename R, typename... Args>
std::shared_ptr<BuiltinFunction<std::function<R(Args...)>>> make_builtin_function(const std::string& functionName,
                                                                                  std::function<R(Args...)> function)
{
	return make_builtin_function_helper<R, Args...>::make_builtin_function(functionName, function);
}
}  // namespace sscript
