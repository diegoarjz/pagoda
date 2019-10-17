#pragma once

#include "argument_value.h"

#include "../value/base_value.h"

#include <functional>
#include <vector>

namespace sscript
{
/**
 * Infers the return and argument types for the function given in \p p, producing a
 * std::function with those types.
 */
template<typename ReturnType, typename... Args>
auto make_function(ReturnType (*p)(Args...)) -> std::function<ReturnType(Args...)>
{
	return {p};
}

/**
 * Infers the return and argument types for the member function given in \p p, producing a
 * std::function with those types.
 */
template<typename ReturnType, class C, typename... Args>
auto make_member_function(ReturnType (C::*p)(Args...)) -> std::function<ReturnType(C, Args...)>
{
	return {p};
}

/**
 * Base template call_function_helper.
 *
 * Specializations take into account different types of argument types and arity.
 */
template<typename R, typename... Args>
struct call_function_helper
{
	static R call(std::function<R(Args...)>& f, const std::vector<BaseValuePtr>& args);
};

/**
 * Specialization of call_function_helper for a single const std:vector<BaseValuePtr>& argument.
 *
 * Useful for variadic functions.
 */
template<typename R>
struct call_function_helper<R, const std::vector<BaseValuePtr>&>
{
	static R call(std::function<R(const std::vector<BaseValuePtr>&)> f, const std::vector<BaseValuePtr>& args)
	{
		return f(args);
	}
};

/**
 * No arguments specialization.
 */
template<typename R>
struct call_function_helper<R>
{
	static R call(std::function<R(void)> f, const std::vector<BaseValuePtr>& args) { return f(); }
};

/**
 * One argument specialization.
 */
template<typename R, typename A1>
struct call_function_helper<R, A1>
{
	static R call(std::function<R(A1)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		return f(a1);
	}
};

/**
 * Two argument specialization.
 */
template<typename R, typename A1, typename A2>
struct call_function_helper<R, A1, A2>
{
	static R call(std::function<R(A1, A2)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		return f(a1, a2);
	}
};

template<typename R, typename A1, typename A2, typename A3>
struct call_function_helper<R, A1, A2, A3>
{
	static R call(std::function<R(A1, A2, A3)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		return f(a1, a2, a3);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4>
struct call_function_helper<R, A1, A2, A3, A4>
{
	static R call(std::function<R(A1, A2, A3, A4)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		return f(a1, a2, a3, a4);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
struct call_function_helper<R, A1, A2, A3, A4, A5>
{
	static R call(std::function<R(A1, A2, A3, A4, A5)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		return f(a1, a2, a3, a4, a5);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct call_function_helper<R, A1, A2, A3, A4, A5, A6>
{
	static R call(std::function<R(A1, A2, A3, A4, A5, A6)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		auto a6 = argument_value<A6>(args[5]);
		return f(a1, a2, a3, a4, a5, a6);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct call_function_helper<R, A1, A2, A3, A4, A5, A6, A7>
{
	static R call(std::function<R(A1, A2, A3, A4, A5, A6, A7)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		auto a6 = argument_value<A6>(args[5]);
		auto a7 = argument_value<A7>(args[6]);
		return f(a1, a2, a3, a4, a5, a6, a7);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7,
         typename A8>
struct call_function_helper<R, A1, A2, A3, A4, A5, A6, A7, A8>
{
	static R call(std::function<R(A1, A2, A3, A4, A5, A6, A7, A8)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		auto a6 = argument_value<A6>(args[5]);
		auto a7 = argument_value<A7>(args[6]);
		auto a8 = argument_value<A8>(args[7]);
		return f(a1, a2, a3, a4, a5, a6, a7, a8);
	}
};

/**
 * Calls the function stored in \p f with \p args and returns its value.
 */
template<typename R, typename... Args>
R call_function(std::function<R(Args...)> f, const std::vector<BaseValuePtr>& args)
{
	return call_function_helper<R, Args...>::call(f, args);
}

/********************
 * Member functions *
 *******************/
template<typename R, class C, typename... Args>
struct call_member_function_helper
{
	static R call(C& instance, std::function<R(Args...)>& f, const std::vector<BaseValuePtr>& args);
};

/**
 * Specialization of call_member_function_helper for a single const std:vector<BaseValuePtr>& argument.
 *
 * Useful for variadic functions.
 */
template<typename R, class C>
struct call_member_function_helper<R, C, const std::vector<BaseValuePtr>&>
{
	static R call(C& instance, std::function<R(const std::vector<BaseValuePtr>&)> f,
	              const std::vector<BaseValuePtr>& args)
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
	static R call(C& instance, std::function<R(C)> f, const std::vector<BaseValuePtr>& args) { return f(instance); }
};

/**
 * One argument specialization.
 */
template<typename R, class C, typename A1>
struct call_member_function_helper<R, C, A1>
{
	static R call(C& instance, std::function<R(C, A1)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		return f(instance, a1);
	}
};

/**
 * Two argument specialization.
 */
template<typename R, class C, typename A1, typename A2>
struct call_member_function_helper<R, C, A1, A2>
{
	static R call(C& instance, std::function<R(C, A1, A2)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		return f(instance, a1, a2);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3>
struct call_member_function_helper<R, C, A1, A2, A3>
{
	static R call(C& instance, std::function<R(C, A1, A2, A3)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		return f(instance, a1, a2, a3);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4>
struct call_member_function_helper<R, C, A1, A2, A3, A4>
{
	static R call(C& instance, std::function<R(C, A1, A2, A3, A4)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		return f(instance, a1, a2, a3, a4);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5>
{
	static R call(C& instance, std::function<R(C, A1, A2, A3, A4, A5)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		return f(instance, a1, a2, a3, a4, a5);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5, A6>
{
	static R call(C& instance, std::function<R(C, A1, A2, A3, A4, A5, A6)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		auto a6 = argument_value<A6>(args[5]);
		return f(instance, a1, a2, a3, a4, a5, a6);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5, A6, A7>
{
	static R call(C& instance, std::function<R(C, A1, A2, A3, A4, A5, A6, A7)> f, const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		auto a6 = argument_value<A6>(args[5]);
		auto a7 = argument_value<A7>(args[6]);
		return f(instance, a1, a2, a3, a4, a5, a6, a7);
	}
};

template<typename R, class C, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7,
         typename A8>
struct call_member_function_helper<R, C, A1, A2, A3, A4, A5, A6, A7, A8>
{
	static R call(C& instance, std::function<R(C, A1, A2, A3, A4, A5, A6, A7, A8)> f,
	              const std::vector<BaseValuePtr>& args)
	{
		auto a1 = argument_value<A1>(args[0]);
		auto a2 = argument_value<A2>(args[1]);
		auto a3 = argument_value<A3>(args[2]);
		auto a4 = argument_value<A4>(args[3]);
		auto a5 = argument_value<A5>(args[4]);
		auto a6 = argument_value<A6>(args[5]);
		auto a7 = argument_value<A7>(args[6]);
		auto a8 = argument_value<A8>(args[7]);
		return f(instance, a1, a2, a3, a4, a5, a6, a7, a8);
	}
};

template<typename R, class C, typename... Args>
R call_member_function(C& instance, std::function<R(C, Args...)> f, const std::vector<BaseValuePtr>& args)
{
	return call_member_function_helper<R, C, Args...>::call(instance, f, args);
}
}  // namespace sscript
