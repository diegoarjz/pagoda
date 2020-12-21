#include "traversal.h"

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

namespace pagoda::graph::traversal
{
Traversal::Traversal(Graph& graph) : m_graph(graph) {}
Traversal::~Traversal() {}

void Traversal::GetInputNodes(const NodePtr& node, std::insert_iterator<NodeSet> inserter)
{
	for (NodePtr n : m_graph.GetNodeInputNodes(node->GetName())) {
		inserter = n;
	}
}

void Traversal::GetOutputNodes(const NodePtr& node, std::insert_iterator<NodeSet> inserter)
{
	for (NodePtr n : m_graph.GetNodeOutputNodes(node->GetName())) {
		inserter = n;
	}
}

void Traversal::GetAdjacentNodes(const NodePtr& node, std::insert_iterator<NodeSet> inserter)
{
	GetInputNodes(node, inserter);
	GetOutputNodes(node, inserter);
}

NodeSet& Traversal::GetNodes() const { return m_graph.getNodes(); }

void Traversal::ForEachUntil(std::function<bool(NodePtr)> handle)
{
	while (HasNext()) {
		if (!handle(Get())) {
			break;
		}
		Advance();
	}
}

void Traversal::ForEach(std::function<void(NodePtr)> handle)
{
	while (HasNext()) {
		handle(Get());
		Advance();
	}
}
}  // namespace pagoda::graph::traversal
