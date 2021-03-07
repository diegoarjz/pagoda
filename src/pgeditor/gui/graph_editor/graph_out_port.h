#pragma once

#include "graph_port.h"

#include <QGraphicsProxyWidget>

namespace pagoda::graph
{
class Node;
using NodePtr = std::shared_ptr<Node>;
}  // namespace pagoda::graph

namespace pgeditor::gui
{
class GraphOutPort : public GraphPort
{
	public:
	GraphOutPort(pagoda::graph::NodePtr node, QGraphicsWidget *parent);
	virtual ~GraphOutPort() {}

	QRectF boundingRect() const override;

	private:
	QGraphicsProxyWidget *m_label;
};
}  // namespace pgeditor::gui
