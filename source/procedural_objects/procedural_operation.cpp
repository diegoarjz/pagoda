#include "procedural_operation.h"

#include "procedural_object_system.h"

#include "common/assertions.h"
#include "procedural_object.h"

#include "../selector.h"

namespace selector
{
ProceduralOperation::ProceduralOperation() : m_parameterContext(std::make_shared<Context>("op")) {}

bool ProceduralOperation::PushProceduralObject(InterfaceName interface, ProceduralObjectPtr procedural_object)
{
	START_PROFILE;

	auto input_interface = input_interfaces.find(interface);
	if (input_interface == input_interfaces.end())
	{
		return false;
	}

	if (!input_interface->second->Accepts(procedural_object))
	{
		return false;
	}

	if (!ValidateProceduralObject(interface, procedural_object))
	{
		return false;
	}

	input_interface->second->AddProceduralObject(procedural_object);
	return true;
}

ProceduralObjectPtr ProceduralOperation::PopProceduralObject(const InterfaceName& interface) const
{
	START_PROFILE;

	auto output_interface = output_interfaces.find(interface);

	if (output_interface == output_interfaces.end())
	{
		return nullptr;
	}

	return output_interface->second->GetAndPopProceduralObject();
}

void ProceduralOperation::CreateInputInterface(const InterfaceName& interfaceName, const ProceduralObjectMask& mask)
{
	START_PROFILE;

	input_interfaces.emplace(
	    std::make_pair(interfaceName, std::make_unique<ProceduralOperationObjectInterface>(interfaceName, mask)));
}

void ProceduralOperation::CreateOutputInterface(const InterfaceName& interfaceName, const ProceduralObjectMask& mask)
{
	START_PROFILE;

	output_interfaces.emplace(
	    std::make_pair(interfaceName, std::make_unique<ProceduralOperationObjectInterface>(interfaceName, mask)));
}

std::shared_ptr<ProceduralObject> ProceduralOperation::GetInputProceduralObject(const InterfaceName& interfaceName)
{
	START_PROFILE;

	auto object = input_interfaces[interfaceName]->GetAndPopProceduralObject();
	m_parameterContext->SetParameter(interfaceName.ToString(), object);

	return object;
}

bool ProceduralOperation::HasInput(const InterfaceName& interfaceName) const
{
	START_PROFILE;

	DBG_ASSERT_MSG(input_interfaces.find(interfaceName) != input_interfaces.end(),
	               "Could not find operation interface");
	return input_interfaces.find(interfaceName)->second->HasProceduralObjects();
}

std::shared_ptr<ProceduralObject> ProceduralOperation::CreateOutputProceduralObject(const InterfaceName& interfaceName)
{
	START_PROFILE;

	auto mask = output_interfaces[interfaceName]->Mask();

	auto procedural_object = Selector::GetInstance().GetProceduralObjectSystem()->CreateProceduralObject(mask);
	output_interfaces[interfaceName]->AddProceduralObject(procedural_object);

	return procedural_object;
}

Parameter ProceduralOperation::GetParameter(const std::string& parameterName) const
{
	return m_parameterContext->GetParameter(parameterName);
}

void ProceduralOperation::SetParameter(const std::string& parameterName, const Parameter& parameter)
{
	m_parameterContext->SetParameter(parameterName, parameter);
}

std::unordered_set<std::string> ProceduralOperation::GetParameterNameList() const
{
	return m_parameterContext->GetParameterNameList();
}

std::unordered_map<std::string, Parameter> ProceduralOperation::GetParameters() const
{
	return m_parameterContext->GetParameters();
}

Parameter ProceduralOperation::ResolveVariable(const Variable& v) const
{
	return m_parameterContext->ResolveVariable(v);
}

}  // namespace selector
