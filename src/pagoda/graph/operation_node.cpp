#include "operation_node.h"

#include "construction_argument_not_found.h"
#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "node_visitor.h"
#include "output_interface_node.h"
#include "unsupported_node_link.h"

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/value_not_found.h>
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

void OperationNode::SetConstructionArguments(
  const std::unordered_map<std::string, DynamicValueBasePtr> &constructionArgs)
{
	auto operationIter = constructionArgs.find("operation");
	if (operationIter == std::end(constructionArgs)) {
		throw ConstructionArgumentNotFound(GetName(), GetId(), "operation");
	}

	auto operation = m_operationFactory->Create(get_value_as<std::string>(*operationIter->second));
	if (operation == nullptr) {
		throw UnknownOperation(get_value_as<std::string>(*operationIter->second));
	}

	SetOperation(operation);
}

void OperationNode::SetOperation(ProceduralOperationPtr operation)
{
	m_operation = operation;
	RegisterOrSetMember("op", m_operation);
}

void OperationNode::AcceptNodeVisitor(NodeVisitor *visitor)
{
	visitor->Visit(std::dynamic_pointer_cast<OperationNode>(shared_from_this()));
}

void OperationNode::Execute(const NodeSet &inNodes, const NodeSet &outNodes)
{
	LOG_TRACE(ProceduralGraph, "Executing OperationNode " << GetName() << "(" << GetId() << ")");
	// Copy all parameters registered in this node to the operation
	for (auto parIter = GetMembersBegin(); parIter != GetMembersEnd(); ++parIter) {
		m_operation->RegisterOrSetMember(parIter->first, GetMember(parIter->first));
	}
}

}  // namespace pagoda::graph
