#include "input_interface_node.h"

#include "graph.h"
#include "graph_execution_context.h"
#include "node.h"
#include "node_set_visitor.h"
#include "output_interface_node.h"

#include <procedural_objects/procedural_operation.h>

namespace selector
{
const char* InputInterfaceNode::name = "InputInterface";

InputInterfaceNode::InputInterfaceNode() : m_interfaceName(0, 0) {}
InputInterfaceNode::~InputInterfaceNode() {}

void InputInterfaceNode::SetInterfaceName(const InterfaceName& interfaceName) { m_interfaceName = interfaceName; }
const InterfaceName& InputInterfaceNode::GetInterfaceName() const { return m_interfaceName; }

void InputInterfaceNode::Execute(GraphExecutionContextPtr executionContext, const NodeSet<Node>& inNodes,
                                 const NodeSet<Node>& outNodes)
{
	START_PROFILE;

	m_proceduralObjects.clear();
	NodeSet<OutputInterfaceNode> outputInterfaceNodes;
	node_type_filter<OutputInterfaceNode>(inNodes, outputInterfaceNodes);

	for (auto outNode : outputInterfaceNodes)
	{
		auto outNodeObjects = outNode->GetProceduralObjects();
		m_proceduralObjects.insert(std::end(m_proceduralObjects), std::begin(outNodeObjects), std::end(outNodeObjects));
	}
}
}  // namespace selector
