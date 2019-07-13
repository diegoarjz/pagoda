#ifndef SELECTOR_PROCEDURAL_GRAPH_BREADTH_FIRST_NODE_VISITOR_H_
#define SELECTOR_PROCEDURAL_GRAPH_BREADTH_FIRST_NODE_VISITOR_H_

#include "node_visitor.h"

namespace selector
{
template<class Delegate_t>
class BreadthFirstNodeVisitor : public NodeVisitor<Delegate_t>
{
public:
	BreadthFirstNodeVisitor(GraphPtr graph, Delegate_t &delegate)
		: NodeVisitor<Delegate_t>(graph, delegate)
	{
	}
	
	void Visit() override
	{
		auto graphInNodes = m_graph->GetGraphInputNodes();
		for (auto &n : graphInNodes)
		{
			m_nodesToVisit.push(n);
		}

		while (!m_nodesToVisit.empty())
		{
			auto n = m_nodesToVisit.front();
			m_nodesToVisit.pop();
			
			m_delegate(n);
		
			auto outNodes = m_graph->GetNodeOutputNodes(n);
			for (auto outNode : outNodes)
			{
				m_nodesToVisit.push(outNode);
			}
		}
	}
private:
	std::queue<NodePtr> m_nodesToVisit;
};
}

#endif