#include "procedural_object.h"

#include "parameter/context.h"

namespace selector
{
ProceduralObject::ProceduralObject() : m_context(std::make_shared<Context>("object")) {}

ProceduralObject::~ProceduralObject() {}

Parameter ProceduralObject::GetParameter(const std::string& parameterName) const
{
	return m_context->GetParameter(parameterName);
}

void ProceduralObject::SetParameter(const std::string& parameterName, const Parameter& parameter)
{
	m_context->SetParameter(parameterName, parameter);
}

std::unordered_set<std::string> ProceduralObject::GetParameterNameList() const
{
	return m_context->GetParameterNameList();
}

std::unordered_map<std::string, Parameter> ProceduralObject::GetParameters() const
{
	return m_context->GetParameters();
}

Parameter ProceduralObject::ResolveVariable(const Variable& v) const
{
	// ProceduralObject variables
	return m_context->ResolveVariable(v);
}
}  // namespace selector
