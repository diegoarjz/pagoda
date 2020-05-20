#ifndef PAGODA_PROCEDURAL_GRAPH_NODE_VISITOR
#define PAGODA_PROCEDURAL_GRAPH_NODE_VISITOR

#include "graph.h"
#include "node.h"

namespace pagoda::graph
{
template<class Delegate_t>
class GraphNodeVisitor
{
public:
	GraphNodeVisitor(Graph& graph, Delegate_t& delegate) : m_graph(graph), m_delegate(delegate) {}

	virtual void Visit()
	{
		for (auto& n : m_graph.GetGraphNodes())
		{
			m_delegate(n);
		}
	}

	const Graph& GetGraph() const { return m_graph; }

protected:
	Graph& m_graph;
	Delegate_t& m_delegate;
};
}  // namespace pagoda::graph

#endif
