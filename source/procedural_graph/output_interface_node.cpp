#include "output_interface_node.h"

#include "graph.h"
#include "graph_execution_context.h"
#include "node.h"
#include "node_execution_result.h"
#include "node_set_visitor.h"
#include "operation_node.h"

#include <procedural_objects/procedural_operation.h>

namespace selector
{
OutputInterfaceNode::OutputInterfaceNode() : m_interfaceName(0, 0) {}
OutputInterfaceNode::~OutputInterfaceNode() {}

void OutputInterfaceNode::SetInterfaceName(const InterfaceName& name) { m_interfaceName = name; }
const InterfaceName& OutputInterfaceNode::GetInterfaceName() const { return m_interfaceName; }

void OutputInterfaceNode::Execute(GraphExecutionContextPtr executionContext, const NodeSet<Node>& inNodes,
                                  const NodeSet<Node>& outNodes)
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
