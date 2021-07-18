#include "operation_node.h"

#include "construction_argument_callback.h"
#include "construction_argument_not_found.h"
#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "output_interface_node.h"
#include "unsupported_node_link.h"

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/value_not_found.h>
#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/operation_factory.h>
#include <pagoda/objects/procedural_operation.h>
#include <pagoda/objects/unknown_operation.h>

using namespace pagoda::dynamic;
using namespace pagoda::objects;

namespace pagoda::graph
{
const char *OperationNode::name = "Operation";

OperationNode::OperationNode(OperationFactoryPtr operationFactory) : m_operationFactory(operationFactory) {}
OperationNode::~OperationNode() {}

void OperationNode::SetConstructionArguments(ConstructionArgumentCallback *cb)
{
	std::string operationName;
	cb->StringArgument("operation", operationName, "Operation");
	auto operation = m_operationFactory->Create(operationName);
	if (operation == nullptr) {
		throw UnknownOperation(operationName);
	}
	SetOperation(operation);
}

void OperationNode::SetExecutionArguments(ExecutionArgumentCallback *cb) { m_operation->SetParameters(cb); }

class InterfaceCreator : public InterfaceCallback
{
	public:
	void InputInterface(InterfacePtr &interface, const std::string &name, const std::string &label,
	                    Interface::Arity arity) override
	{
		interface = std::make_shared<Interface>(name, Interface::Type::Input, arity);
	}

	void OutputInterface(InterfacePtr &interface, const std::string &name, const std::string &label,
	                     Interface::Arity arity) override
	{
		interface = std::make_shared<Interface>(name, Interface::Type::Output, arity);
	}
};

void OperationNode::SetOperation(ProceduralOperationPtr operation)
{
	m_operation = operation;
	InterfaceCreator creator;
	m_operation->Interfaces(&creator);
	RegisterOrSetMember("op", m_operation);
}

void OperationNode::ForEachOperationParameter(
  const std::function<void(const std::string &, const dynamic::DynamicValueBasePtr &)> &f) const
{
	m_operation->ForEachParameter(f);
}

class InterfaceObjectProvider : public InterfaceCallback
{
	public:
	void InputInterface(InterfacePtr &interface, const std::string &name, const std::string &label,
	                    Interface::Arity arity) override
	{
		// std::cout << "Interface " << name << " is " << interface.get() << std::endl;
	}

	void OutputInterface(InterfacePtr &interface, const std::string &name, const std::string &label,
	                     Interface::Arity arity) override
	{
		// std::cout << "Interface " << name << " is " << interface.get() << std::endl;
	}
};

void OperationNode::Execute(const NodeSet &inNodes, const NodeSet &outNodes)
{
	LOG_TRACE(ProceduralGraph, "Executing OperationNode " << GetName() << "(" << GetId() << ")");
	InterfaceObjectProvider provider;
	m_operation->Interfaces(&provider);

	// Copy all parameters registered in this node to the operation
	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd(); ++parIter) {
		m_operation->RegisterOrSetMember(parIter->first, GetMember(parIter->first));
	}
}

const char *const OperationNode::GetNodeType()
{
	static const char *const sNodeType = "Operation";
	return sNodeType;
}

void OperationNode::ForEachConstructionArgument(
  std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> f)
{
	f("operation", std::make_shared<dynamic::String>(m_operation->GetOperationName()));
}

void OperationNode::ForEachExecutionArgument(std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> f)
{
	for (auto p = m_operation->GetMembersBegin(); p != m_operation->GetMembersEnd(); ++p) {
		f(p->first, p->second.m_value);
	}
}
}  // namespace pagoda::graph
