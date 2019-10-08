#include "procedural_object.h"

#include "parameter/context.h"

namespace selector
{
ProceduralObject::ProceduralObject()
    : components(static_cast<size_t>(ComponentType::MaxComponents)),
      procedural_object_mask(),
      m_context(std::make_shared<Context>("object"))
{
}

ProceduralObject::~ProceduralObject() {}

void ProceduralObject::SetComponent(const ComponentType& type, std::shared_ptr<ProceduralComponent> component)
{
	START_PROFILE;

	DBG_ASSERT_MSG(components[static_cast<uint32_t>(type)].expired(), "Trying to overwrite a component already set");
	procedural_object_mask.set(static_cast<uint32_t>(type));
	components[static_cast<uint32_t>(type)] = component;
}

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
