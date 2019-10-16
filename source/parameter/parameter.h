#ifndef SELECTOR_PARAMETER_PARAMETER_H_
#define SELECTOR_PARAMETER_PARAMETER_H_

#include "parameter_exception.h"

#include <string>
#include <variant>

namespace selector
{
class Expression;
using ExpressionPtr = std::shared_ptr<Expression>;

class IParameterizable;
using IParameterizablePtr = std::shared_ptr<IParameterizable>;

using Parameter = std::variant<float, int, bool, std::string, ExpressionPtr, IParameterizablePtr>;

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
template<>
bool parameter_getter<bool>::operator()<std::string>(const std::string &s);

template<>
template<>
float parameter_getter<float>::operator()<ExpressionPtr>(const std::shared_ptr<Expression> &e);

template<>
template<>
int parameter_getter<int>::operator()<ExpressionPtr>(const std::shared_ptr<Expression> &e);

template<>
template<>
std::string parameter_getter<std::string>::operator()<ExpressionPtr>(const ExpressionPtr &e);

/**
 * Returns the underlying value for a \c Parameter.
 * If the \c Parameter is an \c Expression, then it is evaluated and cast to the underlying type.
 * Note: This should always be used to grab the underlying value of a \c Parameter as the \c Parameter implementation
 * is more than likely to be modified. This amortizes the API changes.
 */
template<class T>
T get_parameter_as(const Parameter &p)
{
	parameter_getter<T> getter;
	return std::visit(getter, p);
};

}  // namespace selector

#endif
