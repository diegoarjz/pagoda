#pragma once
#ifndef SELECTOR_PARAMETER_FLOAT_PARAMETER_H_
#define SELECTOR_PARAMETER_FLOAT_PARAMETER_H_

#include "parameter_base.h"

namespace selector
{
template<class T>
class Parameter;

/**
 * Holds float parameter values.
 */
struct FloatParameter
{
	using value_t = float;
	using parameter_t = Parameter<FloatParameter>;
	static const ParameterTypes parameter_type = ParameterTypes::Float;

	FloatParameter() : m_value(0.0f) {}
	value_t m_value;
};  // struct FloatParameter
}  // namespace selector
#endif
