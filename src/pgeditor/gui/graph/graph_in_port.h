#pragma once

#include <QGraphicsProxyWidget>

#include "graph_port.h"

namespace pagoda::graph
{
class InputInterfaceNode;
using InputInterfaceNodePtr = std::shared_ptr<InputInterfaceNode>;
}  // namespace pagoda::graph

namespace pgeditor::gui::graph
{
class GraphNode;

class GraphInPort : public GraphPort
{
	public:
	GraphInPort(GraphNode *node, const pagoda::graph::InputInterfaceNodePtr &interface, QGraphicsWidget *parent);
	virtual ~GraphInPort() {}

	QRectF boundingRect() const override;

	private:
	QGraphicsProxyWidget *m_label;
	pagoda::graph::InputInterfaceNodePtr m_interface;
};

}  // namespace pgeditor::gui::graph
