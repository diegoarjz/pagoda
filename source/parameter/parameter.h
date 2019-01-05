#ifndef SELECTOR_PARAMETER_PARAMETER_H_
#define SELECTOR_PARAMETER_PARAMETER_H_

#include "common/profiler.h"
#include "expression.h"
#include "parameter_base.h"
#include "parameter_identifier.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace selector
{
/**
 * Template class that is capable of holding different types of values.
 */
template<class ParameterType>
class Parameter : public ParameterBase
{
public:
	using value_t = typename ParameterType::value_t;

	explicit Parameter(const ParameterIdentifier& name) : ParameterBase(name) {}
	virtual ~Parameter() {}

	typename ParameterType::value_t SetValue(const typename ParameterType::value_t& new_value)
	{
		auto old_value = m_value.m_value;
		m_value.m_value = new_value;
		return old_value;
	}

	const typename ParameterType::value_t& GetValue() const { return m_value.m_value; }

	typename ParameterType::value_t& GetValue() { return m_value.m_value; }

	ParameterTypes Type() const final { return ParameterType::parameter_type; }

private:
	ParameterType m_value;
};  // class Parameter

}  // namespace selector

#endif
