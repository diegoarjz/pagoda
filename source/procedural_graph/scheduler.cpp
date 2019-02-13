#include "scheduler.h"

#include "graph_execution_context.h"

namespace selector
{
Scheduler::Scheduler(GraphExecutionContextPtr executionContext, std::shared_ptr<Context> &context)
    : m_graphExecutionContext(executionContext), m_graph(executionContext->m_graph), m_graphParameterContext(context)
{
}

void Scheduler::Initialize()
{
	START_PROFILE;

	DBG_ASSERT_MSG(m_executionQueue.empty(), "Initializing Scheduler but execution_queue not empty");
	NodeSet<Node> graph_input_nodes = m_graph->GetGraphInputNodes();

	// Initialize all nodes
	auto graphNodes = m_graph->GetGraphNodes();
	for (auto node : graphNodes)
	{
		DBG_ASSERT_MSG(node->GetParameterContext() != nullptr, "Parameter context should have been set by now");
		node->GetParameterContext()->SetParent(m_graphParameterContext);
	}

	// Compile all expressions
	Compiler compiler(m_graphParameterContext);
	compiler.Compile();

	// Initialize node queue
	for (NodePtr n : graph_input_nodes)
	{
		m_executionQueue.push(n);
	}
}

bool Scheduler::Step()
{
	START_PROFILE;

	if (m_executionQueue.empty())
	{
		return false;
	}

	NodePtr next_node = m_executionQueue.front().lock();
	m_executionQueue.pop();

	auto in_nodes = m_graph->GetNodeInputNodes(next_node);
	auto out_nodes = m_graph->GetNodeOutputNodes(next_node);

	next_node->Execute(m_graphExecutionContext, in_nodes, out_nodes);

	for (NodePtr n : out_nodes)
	{
		auto inserted = m_seenNodes.insert(n);

		if (inserted.second)
		{
			m_executionQueue.push(n);
		}
	}

	return true;
}
}  // namespace selector
