#include "pagoda/objects/procedural_operation.h"

#include "pagoda/objects/hierarchical_component.h"
#include "pagoda/objects/hierarchical_system.h"
#include "pagoda/objects/interface_callback.h"
#include "pagoda/objects/procedural_object.h"
#include "pagoda/objects/procedural_object_system.h"

#include <pagoda/common/debug/assertions.h>

#include <pagoda/dynamic/type_info.h>
#include <pagoda/dynamic/value_not_found.h>
#include <pagoda/dynamic/value_visitor.h>

#include <memory>

using namespace pagoda::dynamic;

namespace pagoda::objects
{
const TypeInfoPtr ProceduralOperation::s_typeInfo =
  std::make_shared<TypeInfo>("ProceduralOperation");

ProceduralOperation::ProceduralOperation(
  ProceduralObjectSystemPtr proceduralObjectSystem)
  : BuiltinClass(s_typeInfo), m_proceduralObjectSystem(proceduralObjectSystem)
{
}

ProceduralOperation::~ProceduralOperation()
{
}

void ProceduralOperation::Execute()
{
	// Set all expression variables to its value
	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd();
	     ++parIter) {
		ExpressionPtr e =
		  std::dynamic_pointer_cast<Expression>(parIter->second.m_value);
		if (e != nullptr) {
			for (const auto& var : e->GetVariables()) {
				try {
					auto variableIdentifiers = var.GetIdentifiers();
					DynamicValueBasePtr nodeParameter =
					  GetMember(variableIdentifiers.front());
					e->SetVariableValue(variableIdentifiers.front(), nodeParameter);
				} catch (ValueNotFoundException& e) {
					LOG_TRACE(ProceduralObjects, "Operation parameter "
					                               << parIter->first
					                               << " not found in Operation.");
				}
			}
		}
	}

	DoWork();
}

ProceduralObjectPtr ProceduralOperation::CreateOutputProceduralObject()
{
	START_PROFILE;

	auto procedural_object = m_proceduralObjectSystem->CreateProceduralObject();

	auto hierarchicalSystem =
	  m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();
	hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(
	  procedural_object);

	return procedural_object;
}

ProceduralObjectPtr ProceduralOperation::CreateOutputProceduralObject(
  ProceduralObjectPtr& base)
{
	auto proceduralObject = m_proceduralObjectSystem->CloneProceduralObject(base);

	auto hierarchicalSystem =
	  m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();
	auto parentObject =
	  hierarchicalSystem->GetComponentAs<HierarchicalComponent>(base);
	auto childObject =
	  hierarchicalSystem->GetComponentAs<HierarchicalComponent>(proceduralObject);

	hierarchicalSystem->SetParent(parentObject, childObject);

	return proceduralObject;
}

std::string ProceduralOperation::ToString() const
{
	return "<ProceduralOperation>";
}

void ProceduralOperation::AcceptVisitor(ValueVisitorBase& visitor)
{
	visitor.Visit(*this);
}

void ProceduralOperation::UpdateValue(const std::string& valueName)
{
	auto expression = std::dynamic_pointer_cast<Expression>(GetValue(valueName));
	if (expression) {
		expression->SetDirty();
	}
}

DynamicValueBasePtr ProceduralOperation::GetValue(const std::string& valueName)
{
	return GetMember(valueName);
}

void ProceduralOperation::ForEachParameter(
  const std::function<void(const std::string&,
                           const dynamic::DynamicValueBasePtr& type)>& f)
{
	for (const auto& pn : m_parameterNames) {
		f(pn, GetMember(pn));
	}
}

}  // namespace pagoda::objects
