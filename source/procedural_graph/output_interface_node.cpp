#include "output_interface_node.h"

#include "construction_argument_not_found.h"
#include "dynamic_value/get_value_as.h"
#include "graph.h"
#include "node.h"
#include "node_set_visitor.h"
#include "operation_node.h"

#include <procedural_objects/procedural_operation.h>

namespace selector
{
const char* OutputInterfaceNode::name = "OutputInterface";

OutputInterfaceNode::OutputInterfaceNode() : m_interfaceName("", 0) {}
OutputInterfaceNode::~OutputInterfaceNode() {}

void OutputInterfaceNode::SetConstructionArguments(
    const std::unordered_map<std::string, DynamicValueBasePtr>& constructionArgs)
{
	auto interfaceNameIter = constructionArgs.find("interface");
	if (interfaceNameIter == std::end(constructionArgs))
	{
		throw ConstructionArgumentNotFound(GetName(), GetId(), "interface");
	}
	auto offsetIter = constructionArgs.find("offset");

	auto interfaceName = get_value_as<std::string>(*interfaceNameIter->second);
	uint16_t offset = 0;
	if (offsetIter != std::end(constructionArgs))
	{
		offset = static_cast<uint16_t>(get_value_as<float>(*offsetIter->second));
	}
	SetInterfaceName(InterfaceName(interfaceName, offset));
}

void OutputInterfaceNode::SetInterfaceName(const InterfaceName& name) { m_interfaceName = name; }
const InterfaceName& OutputInterfaceNode::GetInterfaceName() const { return m_interfaceName; }

void OutputInterfaceNode::Execute(const NodeSet<Node>& inNodes, const NodeSet<Node>& outNodes)
{
	START_PROFILE;

	m_proceduralObjects.clear();
	NodeSet<OperationNode> operationNodes;
	node_type_filter(inNodes, operationNodes);

	NodeSet<OutputInterfaceNode> inputInterfaceNodes;
	node_type_filter(outNodes, inputInterfaceNodes);

	if (operationNodes.size() != 1)
	{
		return;
	}

	auto operationNode = *operationNodes.begin();
	auto proceduralOperation = operationNode->GetOperation();
	ProceduralObjectPtr proceduralObject = nullptr;
	while ((proceduralObject = proceduralOperation->PopProceduralObject(m_interfaceName)))
	{
		m_proceduralObjects.push_back(proceduralObject);
	}
}

}  // namespace selector
