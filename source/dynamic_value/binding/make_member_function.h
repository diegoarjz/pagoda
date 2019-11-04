#pragma once

#include <functional>

namespace selector
{
/**
 * Infers the return and argument types for the member function given in \p p, producing a
 * std::function with those types.
 */
template<typename ReturnType, class C, typename... Args>
auto make_member_function(ReturnType (C::*p)(Args...)) -> std::function<ReturnType(C*, Args...)>
{
	return {p};
}

}  // namespace selector

