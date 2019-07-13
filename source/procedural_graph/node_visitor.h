#ifndef SELECTOR_PROCEDURAL_GRAPH_NODE_VISITOR
#define SELECTOR_PROCEDURAL_GRAPH_NODE_VISITOR

#include "graph.h"
#include "node.h"

namespace selector
{
template<class Delegate_t>
class NodeVisitor
{
public:
	NodeVisitor(GraphPtr graph, Delegate_t &delegate)
		: m_graph(graph), m_delegate(delegate)
	{}
	
	virtual void Visit()
	{
		for (auto &n : m_graph->GetGraphNodes())
		{
			delegate(n);
		}
	}
	
	const GraphPtr GetGraph() const
	{
		return m_graph;
	}
protected:
	GraphPtr m_graph;
	Delegate_t &m_delegate;
};
}

#endif