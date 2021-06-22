#pragma once

#include "graph_node.h"

namespace pgeditor::gui::graph
{
class OperationNode : public GraphNode
{
	Q_OBJECT

	public:
	OperationNode(pagoda::graph::GraphPtr graph, pagoda::graph::NodePtr node);
	~OperationNode() override;

	GraphInPort* GetInPortForNode(pagoda::graph::NodePtr node) const;
	GraphOutPort* GetOutPortForNode(pagoda::graph::NodePtr node) const;

	protected:
	void CreateBody() override;

	private:
	pagoda::graph::NodeSet m_inNodes;
	pagoda::graph::NodeSet m_outNodes;

	std::unordered_map<pagoda::graph::NodePtr, GraphInPort*> m_inInterfaces;
	std::unordered_map<pagoda::graph::NodePtr, GraphOutPort*> m_outInterfaces;
};
}  // namespace pgeditor::gui::graph
