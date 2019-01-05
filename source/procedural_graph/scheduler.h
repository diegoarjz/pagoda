#ifndef SELECTOR_PROCEDURAL_GRAPH_SCHEDULER_H_
#define SELECTOR_PROCEDURAL_GRAPH_SCHEDULER_H_

#include "graph.h"

#include "parameter/compiler.h"
#include "parameter/context.h"

#include <iostream>
#include <queue>
#include <unordered_set>

namespace selector
{
class Scheduler
{
public:
	Scheduler(GraphExecutionContextPtr executionContext, std::shared_ptr<Context> &context)
	    : m_graphExecutionContext(executionContext),
	      m_graph(executionContext->m_graph),
	      m_graphParameterContext(context)
	{
	}

	void Initialize()
	{
		START_PROFILE;

		DBG_ASSERT_MSG(m_executionQueue.empty(), "Initializing Scheduler but execution_queue not empty");
		std::list<NodePtr> graph_input_nodes = m_graph->GetGraphInputNodes();
		std::unordered_set<NodePtr> operation_nodes = m_graph->GetGraphNodesByType(NodeType::Operation);

		// Set parent parameter context to all nodes.
		for (NodePtr n : operation_nodes)
		{
			n->GetParameterContext()->SetParent(m_graphParameterContext);
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

	bool Step()
	{
		START_PROFILE;

		if (m_executionQueue.empty())
		{
			return false;
		}

		NodePtr next_node = m_executionQueue.front().lock();
		m_executionQueue.pop();

		next_node->Execute(m_graphExecutionContext);

		std::list<NodePtr> out_nodes = m_graph->GetNodeOutputNodes(next_node);
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

private:
	using NodeWeakPtrSet = std::unordered_set<NodeWeakPtr, NodeWeakPtrHasher, NodeWeakPtrEqual>;

	std::shared_ptr<GraphExecutionContext> m_graphExecutionContext;
	std::shared_ptr<Graph> m_graph;
	std::shared_ptr<Context> m_graphParameterContext;
	std::queue<NodeWeakPtr> m_executionQueue;
	NodeWeakPtrSet m_seenNodes;
};  // class Scheduler
}  // namespace selector
#endif
