#pragma once

#include "../dynamic_value_base.h"
#include "cast_to.h"

#include <functional>
#include <vector>

namespace pagoda::dynamic
{
/**
 * Base template call_member_function_helper.
 *
 * Specializations take into account different types of argument types and arity.
 */
template<typename R, class C, typename... Args>
struct call_member_function_helper
{
	static R call(C& instance, std::function<R(Args...)>& f, const std::vector<DynamicValueBasePtr>& args);
};

/**
 * Specialization of call_member_function_helper for a single const std:vector<DynamicValueBasePtr>& argument.
 *
 * Useful for variadic functions.
 */
template<typename R, class C>
struct call_member_function_helper<R, C, const std::vector<DynamicValueBasePtr>&>
{
	static R call(C& instance, std::function<R(C*, const std::vector<DynamicValueBasePtr>&)> f,
	              const std::vector<DynamicValueBasePtr>& args)
	{
		return f(instance, args);
	}
};

/**
 * No arguments specialization.
 */
template<typename R, class C>
struct call_member_function_helper<R, C>
{
	static R call(C& instance, std::function<R(C*)> f, const std::vector<DynamicValueBasePtr>& args)
	{
		return f(&instance);
	}
};

/**
 * One argument specialization.
 */
template<typename R, class C, typename A1>
struct call_member_function_helper<R, C, A1>
{
	static R call(C& instance, std::function<R(C*, A1)> f, const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		return f(&instance, a1);
	}
};

/**
 * Two argument specialization.
 */
template<typename R, class C, typename A1, typename A2>
struct call_member_function_helper<R, C, A1, A2>
{
	static R call(C& instance, std::function<R(C*, A1, A2)> f, const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		auto a2 = cast_to<A2>(args[1]);
		return f(&instance, a1, a2);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3>
struct call_member_function_helper<R, C, A1, A2, A3>
{
	static R call(C& instance, std::function<R(C*, A1, A2, A3)> f, const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		auto a2 = cast_to<A2>(args[1]);
		auto a3 = cast_to<A3>(args[2]);
		return f(&instance, a1, a2, a3);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4>
struct call_member_function_helper<R, C, A1, A2, A3, A4>
{
	static R call(C& instance, std::function<R(C*, A1, A2, A3, A4)> f, const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		auto a2 = cast_to<A2>(args[1]);
		auto a3 = cast_to<A3>(args[2]);
		auto a4 = cast_to<A4>(args[3]);
		return f(&instance, a1, a2, a3, a4);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5>
{
	static R call(C& instance, std::function<R(C*, A1, A2, A3, A4, A5)> f, const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		auto a2 = cast_to<A2>(args[1]);
		auto a3 = cast_to<A3>(args[2]);
		auto a4 = cast_to<A4>(args[3]);
		auto a5 = cast_to<A5>(args[4]);
		return f(&instance, a1, a2, a3, a4, a5);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5, A6>
{
	static R call(C& instance, std::function<R(C*, A1, A2, A3, A4, A5, A6)> f,
	              const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		auto a2 = cast_to<A2>(args[1]);
		auto a3 = cast_to<A3>(args[2]);
		auto a4 = cast_to<A4>(args[3]);
		auto a5 = cast_to<A5>(args[4]);
		auto a6 = cast_to<A6>(args[5]);
		return f(&instance, a1, a2, a3, a4, a5, a6);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5, A6, A7>
{
	static R call(C& instance, std::function<R(C*, A1, A2, A3, A4, A5, A6, A7)> f,
	              const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		auto a2 = cast_to<A2>(args[1]);
		auto a3 = cast_to<A3>(args[2]);
		auto a4 = cast_to<A4>(args[3]);
		auto a5 = cast_to<A5>(args[4]);
		auto a6 = cast_to<A6>(args[5]);
		auto a7 = cast_to<A7>(args[6]);
		return f(&instance, a1, a2, a3, a4, a5, a6, a7);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7,
         typename A8>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5, A6, A7, A8>
{
	static R call(C& instance, std::function<R(C*, A1, A2, A3, A4, A5, A6, A7, A8)> f,
	              const std::vector<DynamicValueBasePtr>& args)
	{
		auto a1 = cast_to<A1>(args[0]);
		auto a2 = cast_to<A2>(args[1]);
		auto a3 = cast_to<A3>(args[2]);
		auto a4 = cast_to<A4>(args[3]);
		auto a5 = cast_to<A5>(args[4]);
		auto a6 = cast_to<A6>(args[5]);
		auto a7 = cast_to<A7>(args[6]);
		auto a8 = cast_to<A8>(args[7]);
		return f(&instance, a1, a2, a3, a4, a5, a6, a7, a8);
	}
};

/**
 * Calls the member function stored in \p f on the object \p instance with \p args and returns its value.
 */
template<typename R, class C, typename... Args>
R call_member_function(C& instance, std::function<R(C*, Args...)> f, const std::vector<DynamicValueBasePtr>& args)
{
	return call_member_function_helper<R, C, Args...>::call(instance, f, args);
}
}  // namespace pagoda::dynamic
