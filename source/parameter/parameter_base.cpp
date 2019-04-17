#include "parameter_base.h"

namespace selector
{
ParameterBase::ParameterBase(const ParameterIdentifier& name) : m_parameterName(name), m_expression(nullptr) {}

const std::string& ParameterBase::Name() const { return m_parameterName; }

void ParameterBase::SetExpression(std::shared_ptr<Expression>& e) { m_expression = e; }
std::shared_ptr<Expression> ParameterBase::GetExpression() const { return m_expression; }

}  // namespace selector
