#include "operation_node.h"

#include "graph.h"
#include "graph_execution_context.h"
#include "input_interface_node.h"
#include "node.h"
#include "node_set_visitor.h"

#include <procedural_objects/procedural_operation.h>

namespace selector
{
const char *OperationNode::name = "OperationNode";

OperationNode::OperationNode() {}
OperationNode::~OperationNode() {}

void OperationNode::SetOperation(ProceduralOperationPtr operation) { m_operation = operation; }

void OperationNode::Execute(GraphExecutionContextPtr executionContext, const NodeSet<Node> &inNodes,
                            const NodeSet<Node> &outNodes)
{
	std::shared_ptr<Context> paramContext = GetParameterContext();
	NodeSet<InputInterfaceNode> inputInterfaceNodes;
	node_type_filter<InputInterfaceNode>(inNodes, inputInterfaceNodes);

	for (auto param : paramContext->GetParameters())
	{
		auto parameter = param.second;
		if (parameter->GetExpression() != nullptr)
		{
			parameter->GetExpression()->Evaluate();
		}
	}

	for (auto inInterface : inputInterfaceNodes)
	{
		auto proceduralObjects = inInterface->GetProceduralObjects();
		auto interfaceName = inInterface->GetInterfaceName();

		for (ProceduralObjectPtr o : proceduralObjects)
		{
			m_operation->PushProceduralObject(interfaceName, o);
		}
	}

	// TODO: Is it really necessary to always create an OperationExecutionContext?
	auto ctx = std::make_shared<OperationExecutionContext>();
	ctx->geometry_system = executionContext->m_geometrySystem.lock();
	ctx->procedural_object_system = executionContext->m_proceduralObjectSystem.lock();
	ctx->parameter_context = paramContext;

	m_operation->SetExecutionContext(ctx);
	m_operation->Execute();
}

}  // namespace selector
