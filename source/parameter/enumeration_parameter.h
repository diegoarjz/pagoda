#pragma once
#ifndef SELECTOR_PARAMETER_ENUMERATION_PARAMETER_H_
#define SELECTOR_PARAMETER_ENUMERATION_PARAMETER_H_

#include "parameter_base.h"

namespace selector
{
template<class T>
class Parameter;

/**
 * Holds enumeration parameter values.
 */
template<class Enum>
struct EnumerationParameter
{
	using value_t = typename Enum::Values;
	using parameter_t = Parameter<EnumerationParameter>;
	static const ParameterTypes parameter_type = ParameterTypes::Enumeration;

	EnumerationParameter() : m_value() {}
	value_t m_value;
};  // struct EnumerationParameter
}  // namespace selector
#endif
