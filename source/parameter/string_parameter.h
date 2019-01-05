#pragma once
#ifndef SELECTOR_PARAMETER_STRING_PARAMETER_H_
#define SELECTOR_PARAMETER_STRING_PARAMETER_H_

#include "parameter_base.h"

namespace selector
{
template<class T>
class Parameter;

/**
 * Holds string parameter values.
 */
struct StringParameter
{
	using value_t = std::string;
	using parameter_t = Parameter<StringParameter>;
	static const ParameterTypes parameter_type = ParameterTypes::String;

	StringParameter() : m_value("") {}
	value_t m_value;
};  // struct StringParameter
}  // namespace selector

#endif
