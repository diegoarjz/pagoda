#include "scheduler.h"

#include "breadth_first_node_visitor.h"
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

	// Initialize node queue
	struct Delegate
	{
		Delegate(GraphPtr graph) : m_graph(graph), m_maxDepth(0) {}

		void operator()(NodePtr n)
		{
			auto seenNodeIter = m_nodeDepths.find(n);
			if (seenNodeIter == m_nodeDepths.end())
			{
				seenNodeIter = m_nodeDepths.emplace(n, 0).first;
			}

			auto thisNodeDepth = seenNodeIter->second;
			m_maxDepth = std::max(m_maxDepth, thisNodeDepth);
			for (auto outNode : m_graph->GetNodeOutputNodes(n))
			{
				if (m_nodeDepths[outNode] <= thisNodeDepth)
				{
					m_nodeDepths[outNode] = thisNodeDepth + 1;
				}
			}
		}

		std::unordered_map<NodePtr, uint32_t> m_nodeDepths;
		GraphPtr m_graph;
		uint32_t m_maxDepth;
	};

	Delegate d(m_graph);
	BreadthFirstNodeVisitor<Delegate> v(m_graph, d);
	v.Visit();

	m_currentBucket = 0;
	m_executionQueue.resize(d.m_maxDepth + 1);
	for (auto n : d.m_nodeDepths)
	{
		m_executionQueue[n.second].push_back(n.first);
	}
}

bool Scheduler::Step()
{
	START_PROFILE;

	if (m_currentBucket >= m_executionQueue.size())
	{
		return false;
	}

	auto &nextQueue = m_executionQueue.at(m_currentBucket);
	auto nextNode = nextQueue.front().lock();
	nextQueue.pop_front();

	if (nextQueue.empty())
	{
		++m_currentBucket;
	}

	auto inNodes = m_graph->GetNodeInputNodes(nextNode);
	auto outNodes = m_graph->GetNodeOutputNodes(nextNode);

	nextNode->SetExpressionVariables();
	nextNode->Execute(m_graphExecutionContext, inNodes, outNodes);

	/*
	NodePtr next_node = m_executionQueue.front().lock();
	m_executionQueue.pop();

	auto in_nodes = m_graph->GetNodeInputNodes(next_node);
	auto out_nodes = m_graph->GetNodeOutputNodes(next_node);

	next_node->SetExpressionVariables();
	next_node->Execute(m_graphExecutionContext, in_nodes, out_nodes);
	*/

	/*
	for (NodePtr n : out_nodes)
	{
	    auto inserted = m_seenNodes.insert(n);

	    if (inserted.second)
	    {
	        m_executionQueue.push(n);
	    }
	}
	*/

	return true;
}
}  // namespace selector
