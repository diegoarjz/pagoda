#include "procedural_operation.h"

#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_object.h"
#include "procedural_object_system.h"

#include <pagoda/common/debug/assertions.h>

#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/value_not_found.h>
#include <pagoda/dynamic/value_visitor.h>

#include <memory>

using namespace pagoda::dynamic;

namespace pagoda::objects
{
const TypeInfoPtr ProceduralOperation::s_typeInfo = std::make_shared<TypeInfo>("ProceduralOperation");

ProceduralOperation::ProceduralOperation(ProceduralObjectSystemPtr proceduralObjectSystem)
  : BuiltinClass(s_typeInfo), m_proceduralObjectSystem(proceduralObjectSystem), m_needsUpdate(false),
    m_pendingObjects(0), m_processedObjects(0)
{
}

void ProceduralOperation::Execute()
{
	// Set all expression variables to its value
	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd(); ++parIter) {
		ExpressionPtr e = std::dynamic_pointer_cast<Expression>(parIter->second.m_value);
		if (e != nullptr) {
			for (const auto& var : e->GetVariables()) {
				try {
					auto variableIdentifiers = var.GetIdentifiers();
					DynamicValueBasePtr nodeParameter = GetMember(variableIdentifiers.front());
					e->SetVariableValue(variableIdentifiers.front(), nodeParameter);
				} catch (ValueNotFoundException& e) {
					LOG_TRACE(ProceduralObjects, "Operation parameter " << parIter->first << " not found in Operation.");
				}
			}
		}
	}

	DoWork();
	m_needsUpdate = false;
}

bool ProceduralOperation::PushProceduralObject(const std::string& interface, ProceduralObjectPtr procedural_object)
{
	START_PROFILE;

	++m_pendingObjects;
	m_inputInterfaces[interface].Add(procedural_object);

	if (!m_needsUpdate) {
		m_needUpdateHandlers(this);
		m_needsUpdate = true;
	}

	return true;
}

ProceduralObjectPtr ProceduralOperation::PopProceduralObject(const std::string& interface)
{
	START_PROFILE;
	return m_outputInterfaces[interface].GetFront();
}

void ProceduralOperation::CreateInputInterface(const std::string& interfaceName)
{
	START_PROFILE;

	m_inputInterfaces.emplace(std::make_pair(interfaceName, Interface{}));
}

void ProceduralOperation::CreateOutputInterface(const std::string& interfaceName)
{
	START_PROFILE;

	m_outputInterfaces.emplace(std::make_pair(interfaceName, Interface{}));
}

std::shared_ptr<ProceduralObject> ProceduralOperation::GetInputProceduralObject(const std::string& interfaceName)
{
	START_PROFILE;

	auto iter = m_inputInterfaces.find(interfaceName);
	if (iter == m_inputInterfaces.end()) {
		return nullptr;
	}

	auto object = iter->second.GetFront();
	RegisterOrSetMember(interfaceName, object);

	--m_pendingObjects;
	++m_processedObjects;

	m_progressHandlers(m_processedObjects, m_pendingObjects);

	return object;
}

bool ProceduralOperation::HasInput(const std::string& interfaceName) const
{
	START_PROFILE;

	DBG_ASSERT_MSG(m_inputInterfaces.find(interfaceName) != m_inputInterfaces.end(),
	               "Could not find operation interface");
	return m_inputInterfaces.find(interfaceName)->second.HasObjects();
}

std::shared_ptr<ProceduralObject> ProceduralOperation::CreateOutputProceduralObject(const std::string& interfaceName)
{
	START_PROFILE;

	auto procedural_object = m_proceduralObjectSystem->CreateProceduralObject();
	m_outputInterfaces[interfaceName].Add(procedural_object);

	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();
	hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(procedural_object);

	m_outputInterfaces[interfaceName].m_signal(this, interfaceName, procedural_object);

	return procedural_object;
}

std::shared_ptr<ProceduralObject> ProceduralOperation::CreateOutputProceduralObject(
  std::shared_ptr<ProceduralObject>& base, const std::string& interfaceName)
{
	auto proceduralObject = m_proceduralObjectSystem->CloneProceduralObject(base);
	m_outputInterfaces[interfaceName].Add(proceduralObject);

	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();
	auto parentObject = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(base);
	auto childObject = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(proceduralObject);

	hierarchicalSystem->SetParent(parentObject, childObject);

	m_outputInterfaces[interfaceName].m_signal(this, interfaceName, proceduralObject);

	return proceduralObject;
}

std::string ProceduralOperation::ToString() const { return "<ProceduralOperation>"; }

void ProceduralOperation::AcceptVisitor(ValueVisitorBase& visitor) { visitor.Visit(*this); }

void ProceduralOperation::UpdateValue(const std::string& valueName)
{
	auto expression = std::dynamic_pointer_cast<Expression>(GetValue(valueName));
	if (expression) {
		expression->SetDirty();
	}
}

DynamicValueBasePtr ProceduralOperation::GetValue(const std::string& valueName) { return GetMember(valueName); }

bool ProceduralOperation::HasInputInterface(const std::string& name) const
{
	return m_inputInterfaces.find(name) != m_inputInterfaces.end();
}

bool ProceduralOperation::HasOutputInterface(const std::string& name) const
{
	return m_outputInterfaces.find(name) != m_outputInterfaces.end();
}

void ProceduralOperation::LinkInputInterface(const std::string& inputInterface, const std::string& outputInterface,
                                             const std::shared_ptr<ProceduralOperation>& op)
{
	op->OnOutputObjectCreated(outputInterface,
	                          [&](ProceduralOperation* op, const std::string& name, ProceduralObjectPtr object) {
		                          PushProceduralObject(inputInterface, object);
	                          });
}

void ProceduralOperation::OnOutputObjectCreated(const std::string& interface,
                                                const std::function<InterfaceHandler_t>& handler)
{
	m_outputInterfaces[interface].m_signal.connect(handler);
}

void ProceduralOperation::OnProgress(const std::function<void(const std::size_t&, const std::size_t)>& handler)
{
	m_progressHandlers.connect(handler);
}

void ProceduralOperation::OnNeedsUpdate(const std::function<void(ProceduralOperation*)>& handler)
{
	m_needUpdateHandlers.connect(handler);
}

void ProceduralOperation::ForEachInputInterface(const std::function<void(const std::string&)>& f)
{
	for (const auto& i : m_inputInterfaces) {
		f(i.first);
	}
}

void ProceduralOperation::ForEachOutputInterface(const std::function<void(const std::string&)>& f)
{
	for (const auto& i : m_outputInterfaces) {
		f(i.first);
	}
}

void ProceduralOperation::ForEachParameter(
  const std::function<void(const std::string&, const dynamic::DynamicValueBasePtr& type)>& f)
{
	for (const auto& pn : m_parameterNames) {
		f(pn, GetMember(pn));
	}
}

}  // namespace pagoda::objects
