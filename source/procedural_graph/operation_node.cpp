#include "operation_node.h"

#include "construction_argument_not_found.h"
#include "dynamic_value/get_value_as.h"
#include "dynamic_value/value_not_found.h"
#include "graph.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"
#include "node_visitor.h"
#include "output_interface_node.h"
#include "unsupported_node_link.h"

#include "procedural_objects/operation_factory.h"
#include "procedural_objects/procedural_operation.h"
#include "procedural_objects/unknown_operation.h"

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
		throw ConstructionArgumentNotFound(GetName(), GetId(), "operation");
	}

	auto operation = m_operationFactory->Create(get_value_as<std::string>(*operationIter->second));
	if (operation == nullptr)
	{
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

namespace
{
class out_visitor : public NodeVisitor
{
public:
	out_visitor(ProceduralOperationPtr operation) : m_operation(operation) {}

	void Visit(std::shared_ptr<OperationNode> n) override { throw UnsupportedNodeLink("output", "OperationNode"); }

	void Visit(std::shared_ptr<InputInterfaceNode> n) override
	{
		throw UnsupportedNodeLink("output", "InputInterfaceNode");
	}

	void Visit(std::shared_ptr<OutputInterfaceNode> n) override
	{
		auto interface = n->GetInterfaceName();
		ProceduralObjectPtr object = nullptr;
		while ((object = m_operation->PopProceduralObject(interface)) != nullptr)
		{
			n->AddProceduralObject(object);
		}
	}

	void Visit(std::shared_ptr<ParameterNode> n) override { throw UnsupportedNodeLink("output", "ParameterNode"); }

	ProceduralOperationPtr m_operation;
};
}  // namespace

void OperationNode::Execute(const NodeSet<Node> &inNodes, const NodeSet<Node> &outNodes)
{
	LOG_TRACE(ProceduralGraph, "Executing OperationNode " << GetName() << "(" << GetId() << ")");
	for (auto parIter = m_operation->GetMembersBegin(); parIter != m_operation->GetMembersEnd(); ++parIter)
	{
		try
		{
			auto nodeParameter = GetMember(parIter->first);
			m_operation->SetMember(parIter->first, GetMember(parIter->first));
		}
		catch (ValueNotFoundException &e)
		{
			LOG_TRACE(ProceduralGraph, "Operation parameter " << parIter->first << " not found in Node " << GetName());
		}
	}

	m_operation->Execute();

	out_visitor v(m_operation);
	for (auto n : outNodes)
	{
		n->AcceptNodeVisitor(&v);
	}
}

}  // namespace selector
