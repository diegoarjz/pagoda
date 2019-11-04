#ifndef SELECTOR_PARAMETER_PARAMETER_H_
#define SELECTOR_PARAMETER_PARAMETER_H_

#include "parameter_exception.h"

#include "dynamic_value/dynamic_value_base.h"
#include "dynamic_value/get_value_as.h"

#include <string>
#include <variant>

namespace selector
{
/**
 * Returns the underlying value for a \c DynamicValueBasePtr.
 * If the \c DynamicValueBasePtr is an \c Expression, then it is evaluated and cast to the underlying type.
 * Note: This should always be used to grab the underlying value of a \c DynamicValueBasePtr as the \c
 * DynamicValueBasePtr implementation is more than likely to be modified. This amortizes the API changes.
 */
template<class T>
T get_parameter_as(const DynamicValueBasePtr &p)
{
	throw std::runtime_error("Unimplemented");
	/*
	parameter_getter<T> getter;
	return std::visit(getter, p);
	*/
};

}  // namespace selector

#endif
