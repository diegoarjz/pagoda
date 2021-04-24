#pragma once

#include "graph_port.h"

#include <QGraphicsProxyWidget>

namespace pagoda::graph
{
class OutputInterfaceNode;
using OutputInterfaceNodePtr = std::shared_ptr<OutputInterfaceNode>;
}  // namespace pagoda::graph

namespace pgeditor::gui::graph
{
class GraphNode;

class GraphOutPort : public GraphPort
{
	public:
	GraphOutPort(GraphNode *node, const pagoda::graph::OutputInterfaceNodePtr &interface, QGraphicsWidget *parent);
	virtual ~GraphOutPort() {}

	QRectF boundingRect() const override;

	private:
	QGraphicsProxyWidget *m_label;
	pagoda::graph::OutputInterfaceNodePtr m_interface;
};
}  // namespace pgeditor::gui::graph
