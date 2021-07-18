#include "parameter.h"

namespace pagoda::objects
{
ParameterBase::ParameterBase(const std::string& name)
  : m_name{name}, m_label{name}
{
}

ParameterBase::ParameterBase(const std::string& name, const std::string& label)
  : m_name{name}, m_label{label}
{
}

ParameterBase::~ParameterBase()
{
}

bool ParameterBase::HasFlag(Flag flag) const
{
	return m_flags.test(static_cast<size_t>(flag));
}

void ParameterBase::SetFlag(Flag flag, bool val)
{
	m_flags.set(static_cast<size_t>(flag), val);
}

////////////////////////////////////////
/// \name Getters and Setters
////////////////////////////////////////
const std::string& ParameterBase::GetName() const
{
	return m_name;
}
const std::string& ParameterBase::GetLabel() const
{
	return m_label;
}

////////////////////////////////////////
// \name Conversion and Serialization
////////////////////////////////////////

template<>
dynamic::DynamicValueBasePtr Parameter<std::string>::ToDynamicValue()
{
	return std::make_shared<dynamic::String>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<float>::ToDynamicValue()
{
	return std::make_shared<dynamic::FloatValue>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<int>::ToDynamicValue()
{
	return std::make_shared<dynamic::Integer>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<bool>::ToDynamicValue()
{
	return std::make_shared<dynamic::Boolean>(*m_valuePtr);
}

template<>
dynamic::DynamicValueBasePtr Parameter<math::Plane<float>>::ToDynamicValue()
{
	return std::make_shared<dynamic::DynamicPlane>(*m_valuePtr);
}

}  // namespace pagoda::objects
