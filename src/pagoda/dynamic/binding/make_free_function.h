#pragma once

#include <functional>

namespace pagoda::dynamic
{
/**
 * Infers the return and argument types for the function given in \p p, creating a
 * std::function with those types.
 */
template<typename ReturnType, typename... Args>
auto make_free_function(ReturnType (*p)(Args...)) -> std::function<ReturnType(Args...)>
{
	return {p};
}
}  // namespace pagoda::dynamic
