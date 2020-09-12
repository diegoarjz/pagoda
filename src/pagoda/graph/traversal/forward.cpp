#include "forward.h"

#include <pagoda/graph/query/input_node.h>
#include <iterator>

#include <pagoda/graph/graph.h>

namespace pagoda::graph::traversal
{
Forward::Forward(Graph& graph) : Traversal(graph)
{
	query::InputNode q(graph, [this](NodePtr n) { m_nodesToVisit.push(n); });
	graph.ExecuteQuery(q);
}

NodePtr Forward::Get() { return m_nodesToVisit.front(); }

bool Forward::Advance()
{
	auto front = m_nodesToVisit.front();
	m_nodesToVisit.pop();
	NodeSet outNodes;
	GetOutputNodes(front, std::inserter(outNodes, std::end(outNodes)));
	for (auto n : outNodes) {
		m_nodesToVisit.push(n);
	}
	return HasNext();
}

bool Forward::HasNext() { return m_nodesToVisit.size() > 0; }

}  // namespace pagoda::graph::traversal
