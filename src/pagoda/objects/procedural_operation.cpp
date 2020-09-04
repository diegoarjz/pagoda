#include "procedural_operation.h"

#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_object.h"
#include "procedural_object_system.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/value_visitor.h>

using namespace pagoda::dynamic;

namespace pagoda::objects
{
const TypeInfoPtr ProceduralOperation::s_typeInfo = std::make_shared<TypeInfo>("ProceduralOperation");

ProceduralOperation::ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem)
    : BuiltinClass(s_typeInfo),
      m_proceduralObjectSystem(proceduralObjectSystem),
      m_pendingObjects(0),
      m_processedObjects(0)
{
}

void ProceduralOperation::Execute() { DoWork(); }

bool ProceduralOperation::PushProceduralObject(const std::string& interface, ProceduralObjectPtr procedural_object)
{
	START_PROFILE;

	/*
	auto input_interface = input_interfaces.find(interface);
	if (input_interface == input_interfaces.end())
	{
	    return false;
	}
	*/

	++m_pendingObjects;
	input_interfaces[interface].push_back(procedural_object);
	return true;
}

ProceduralObjectPtr ProceduralOperation::PopProceduralObject(const std::string& interface)
{
	START_PROFILE;

	/*
	auto output_interface = output_interfaces.find(interface);
	if (output_interface == output_interfaces.end() || output_interface->second.empty())
	{
	    return nullptr;
	}
	*/

	if (output_interfaces[interface].empty())
	{
		return nullptr;
	}
	auto object = output_interfaces[interface].front();
	output_interfaces[interface].pop_front();

	return object;
}

void ProceduralOperation::CreateInputInterface(const std::string& interfaceName)
{
	START_PROFILE;

	input_interfaces.emplace(std::make_pair(interfaceName, std::list<ProceduralObjectPtr>()));
}

void ProceduralOperation::CreateOutputInterface(const std::string& interfaceName)
{
	START_PROFILE;

	output_interfaces.emplace(std::make_pair(interfaceName, std::list<ProceduralObjectPtr>()));
}

std::shared_ptr<ProceduralObject> ProceduralOperation::GetInputProceduralObject(const std::string& interfaceName)
{
	START_PROFILE;

	auto object = input_interfaces[interfaceName].front();
	input_interfaces[interfaceName].pop_front();
	RegisterOrSetMember(interfaceName, object);

	--m_pendingObjects;
	++m_processedObjects;

	m_progressHandlers(m_processedObjects, m_pendingObjects);

	return object;
}

bool ProceduralOperation::HasInput(const std::string& interfaceName) const
{
	START_PROFILE;

	DBG_ASSERT_MSG(input_interfaces.find(interfaceName) != input_interfaces.end(),
	               "Could not find operation interface");
	return !input_interfaces.find(interfaceName)->second.empty();
}

std::shared_ptr<ProceduralObject> ProceduralOperation::CreateOutputProceduralObject(const std::string& interfaceName)
{
	START_PROFILE;

	auto procedural_object = m_proceduralObjectSystem->CreateProceduralObject();
	output_interfaces[interfaceName].push_back(procedural_object);

	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();
	hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(procedural_object);

	m_outputObjectCreated[interfaceName](this, interfaceName, procedural_object);
	std::cout << "Created output on interface " << interfaceName << std::endl;

	return procedural_object;
}

std::shared_ptr<ProceduralObject> ProceduralOperation::CreateOutputProceduralObject(
    std::shared_ptr<ProceduralObject>& base, const std::string& interfaceName)
{
	auto proceduralObject = m_proceduralObjectSystem->CloneProceduralObject(base);
	output_interfaces[interfaceName].push_back(proceduralObject);

	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();
	auto parentObject = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(base);
	auto childObject = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(proceduralObject);

	hierarchicalSystem->SetParent(parentObject, childObject);

	m_outputObjectCreated[interfaceName](this, interfaceName, proceduralObject);

	std::cout << "Created output on interface '" << interfaceName << "'" << std::endl;

	return proceduralObject;
}

std::string ProceduralOperation::ToString() const { return "<ProceduralOperation>"; }

void ProceduralOperation::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

void ProceduralOperation::RegisterValues(const std::unordered_map<std::string, DynamicValueBasePtr>& values)
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

DynamicValueBasePtr ProceduralOperation::GetValue(const std::string& valueName) { return GetMember(valueName); }

void ProceduralOperation::LinkInputInterface(const std::string& inputInterface, const std::string& outputInterface,
                                             const std::shared_ptr<ProceduralOperation>& op)
{
	m_inputOperations[inputInterface].push_back(op.get());
	op->m_outputOperations[outputInterface].push_back(this);
	op->OnOutputObjectCreated(outputInterface,
	                          [&](ProceduralOperation* op, const std::string& name, ProceduralObjectPtr object) {
		                          PushProceduralObject(inputInterface, object);
	                          });
}

void ProceduralOperation::OnOutputObjectCreated(
    const std::string& interface,
    const std::function<void(ProceduralOperation*, const std::string&, ProceduralObjectPtr)>& handler)
{
	m_outputObjectCreated[interface].connect(handler);
}

void ProceduralOperation::OnProgress(const std::function<void(const std::size_t&, const std::size_t)>& handler)
{
	m_progressHandlers.connect(handler);
}

}  // namespace pagoda::objects
