#include "scheduler.h"

#include "breadth_first_node_visitor.h"
#include "graph_execution_context.h"

namespace selector
{
Scheduler::Scheduler(GraphExecutionContextPtr executionContext, std::shared_ptr<Context> &context)
    : m_graphExecutionContext(executionContext),
      m_graph(executionContext->m_graph),
      m_graphParameterContext(context),
      m_executionQueue(m_graph)
{
}

void Scheduler::Initialize()
{
	START_PROFILE;

	NodeSet<Node> graph_input_nodes = m_graph->GetGraphInputNodes();

	// Initialize all nodes
	auto graphNodes = m_graph->GetGraphNodes();
	for (auto node : graphNodes)
	{
		DBG_ASSERT_MSG(node->GetParameterContext() != nullptr, "Parameter context should have been set by now");
		node->GetParameterContext()->SetParent(m_graphParameterContext);
	}
}

bool Scheduler::Step()
{
	START_PROFILE;

	auto nextNode = m_executionQueue.GetNextNode();
	if (nextNode == nullptr)
	{
		return false;
	}

	auto inNodes = m_graph->GetNodeInputNodes(nextNode);
	auto outNodes = m_graph->GetNodeOutputNodes(nextNode);

	nextNode->SetExpressionVariables();
	nextNode->Execute(m_graphExecutionContext, inNodes, outNodes);

	return true;
}
}  // namespace selector
