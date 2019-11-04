#include "operation_node.h"

#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "parameter/parameter.h"

#include "procedural_objects/operation_factory.h"
#include "procedural_objects/procedural_operation.h"

namespace selector
{
const char *OperationNode::name = "Operation";

OperationNode::OperationNode(OperationFactoryPtr operationFactory) : m_operationFactory(operationFactory) {}
OperationNode::~OperationNode() {}

void OperationNode::SetConstructionArguments(
    const std::unordered_map<std::string, DynamicValueBasePtr> &constructionArgs)
{
	auto operationIter = constructionArgs.find("operation");
	if (operationIter == std::end(constructionArgs))
	{
		// TODO: throw
	}

	auto operation = m_operationFactory->Create(get_parameter_as<std::string>(operationIter->second));
	if (operation == nullptr)
	{
		// TODO: throw
	}

	SetOperation(operation);
}

void OperationNode::SetOperation(ProceduralOperationPtr operation) { m_operation = operation; }

void OperationNode::Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
	NodeSet<InputInterfaceNode> inputInterfaceNodes;
	node_type_filter<InputInterfaceNode>(inNodes, inputInterfaceNodes);

	for (auto inInterface : inputInterfaceNodes)
	{
		auto proceduralObjects = inInterface->GetProceduralObjects();
		auto interfaceName = inInterface->GetInterfaceName();

		for (ProceduralObjectPtr o : proceduralObjects)
		{
			m_operation->PushProceduralObject(interfaceName, o);
		}
	}

	throw std::runtime_error("Unimplemented");
	/*
	const auto &operationParameters = m_operation->GetParameterNameList();
	for (const auto &parameterName : operationParameters)
	{
	    m_operation->SetMember(parameterName, GetMember(parameterName));
	}
	*/
	// paramContext->SetParameter("op", m_operation);
	// paramContext->UpdateExpressions();

	m_operation->Execute();
}

}  // namespace selector
