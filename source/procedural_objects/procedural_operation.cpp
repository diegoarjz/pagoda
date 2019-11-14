#include "procedural_operation.h"

#include "procedural_object_system.h"

#include "common/assertions.h"
#include "procedural_object.h"

#include "../selector.h"

namespace selector
{
const TypeInfoPtr ProceduralOperation::s_typeInfo = std::make_shared<TypeInfo>("ProceduralOperation");

ProceduralOperation::ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem)
    : BuiltinClass(s_typeInfo), m_proceduralObjectSystem(proceduralObjectSystem)
{
}

void ProceduralOperation::Execute()
{
    DoWork();
}

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

void ProceduralOperation::CreateInputInterface(const InterfaceName& interfaceName)
{
	START_PROFILE;

	input_interfaces.emplace(
	    std::make_pair(interfaceName, std::make_unique<ProceduralOperationObjectInterface>(interfaceName)));
}

void ProceduralOperation::CreateOutputInterface(const InterfaceName& interfaceName)
{
	START_PROFILE;

	output_interfaces.emplace(
	    std::make_pair(interfaceName, std::make_unique<ProceduralOperationObjectInterface>(interfaceName)));
}

std::shared_ptr<ProceduralObject> ProceduralOperation::GetInputProceduralObject(const InterfaceName& interfaceName)
{
	START_PROFILE;

	auto object = input_interfaces[interfaceName]->GetAndPopProceduralObject();
    RegisterOrSetMember(interfaceName.ToString(), object);

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

	auto procedural_object = m_proceduralObjectSystem->CreateProceduralObject();
	output_interfaces[interfaceName]->AddProceduralObject(procedural_object);

	return procedural_object;
}

std::string ProceduralOperation::ToString() const { return "<ProceduralOperation>"; }

void ProceduralOperation::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

void ProceduralOperation::RegisterValues(const std::unordered_map<std::string, DynamicValueBasePtr> &values)
{
    for (const auto& v : values)
    {
        RegisterMember(v.first, v.second);
    }
}

void ProceduralOperation::UpdateValue(const std::string& valueName)
{
	auto expression = std::dynamic_pointer_cast<Expression>(GetValue(valueName));
    if (expression)
    {
        expression->SetDirty();
    }
}

DynamicValueBasePtr ProceduralOperation::GetValue(const std::string& valueName)
{
    return GetMember(valueName);
}

}  // namespace selector
