#ifndef SELECTOR_PARAMETER_PARAMETER_H_
#define SELECTOR_PARAMETER_PARAMETER_H_

#include "expression.h"
#include "parameter_exception.h"

#include <string>
#include <variant>

namespace selector
{
using Parameter = std::variant<float, std::string, ExpressionPtr>;

template<typename T>
struct parameter_getter
{
	T operator()(const T &p) { return p; }

	T operator()(const Expression &e);

	template<typename V>
	T operator()(const V &v)
	{
		throw ParameterException("Cannot convert parameter");
	}
};

template<>
float parameter_getter<float>::operator()(const Expression &e);

template<>
std::string parameter_getter<std::string>::operator()(const Expression &e);

/**
 * Returns the underlying value for a \c Parameter.
 * If the \c Parameter is an \c Expression, then it is evaluated and cast to the underlying type.
 * Note: This should always be used to grab the underlying value of a \c Parameter as the \c Parameter implementation
 * is more than likely to be modified. This amortizes the API changes.
 */
template<class T>
T get_parameter_as(Parameter &p)
{
	parameter_getter<T> getter;
	return std::visit(getter, p);
};

}  // namespace selector

#endif
