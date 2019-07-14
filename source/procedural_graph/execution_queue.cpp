#include "execution_queue.h"

#include "breadth_first_node_visitor.h"
#include "graph.h"
#include "node.h"

#include <unordered_map>

namespace selector
{
class ExecutionQueue::Impl
{
public:
	Impl(GraphPtr graph) : m_graph(graph)
	{
		m_orderedNodes.reserve(graph->GetGraphNodes().size());
		BreadthFirstNodeVisitor<ExecutionQueue::Impl> visitor(graph, *this);
		visitor.Visit();
		std::sort(m_orderedNodes.begin(), m_orderedNodes.end(),
		          [this](NodePtr a, NodePtr b) { return this->m_nodeDepths[a] < this->m_nodeDepths[b]; });
		Reset();
	}

	NodePtr GetNextNode()
	{
		if (m_currentIterator == m_orderedNodes.end())
		{
			return nullptr;
		}
		return *m_currentIterator++;
	}

	void Reset() { m_currentIterator = m_orderedNodes.begin(); }

	std::size_t GetNodeCount() const { return m_orderedNodes.size(); }

	void operator()(NodePtr n)
	{
		m_orderedNodes.push_back(n);
		auto thisNodeDepth = m_nodeDepths.emplace(n, 0).first->second;
		for (auto outNode : m_graph.lock()->GetNodeOutputNodes(n))
		{
			if (m_nodeDepths[outNode] <= thisNodeDepth)
			{
				m_nodeDepths[outNode] = thisNodeDepth + 1;
			}
		}
	}

	std::weak_ptr<Graph> m_graph;
	std::unordered_map<NodePtr, uint32_t> m_nodeDepths;
	std::vector<NodePtr> m_orderedNodes;
	std::vector<NodePtr>::iterator m_currentIterator;
};

ExecutionQueue::ExecutionQueue(GraphPtr graph) : m_implementation(std::make_unique<ExecutionQueue::Impl>(graph))
{
	START_PROFILE;
}

ExecutionQueue::~ExecutionQueue() {}

NodePtr ExecutionQueue::GetNextNode() { return m_implementation->GetNextNode(); }

void ExecutionQueue::Reset() { m_implementation->Reset(); }

std::size_t ExecutionQueue::GetNodeCount() const { return m_implementation->GetNodeCount(); }
}  // namespace selector
