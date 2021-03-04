#pragma once

#include <QGraphicsProxyWidget>

#include "graph_port.h"
//#include "graph_port_connection.h"

namespace pgeditor::gui
{
class GraphInPort : public GraphPort
{
	public:
	GraphInPort(pagoda::graph::NodePtr node, QGraphicsWidget *parent);
	virtual ~GraphInPort() {}

	QRectF boundingRect() const override;

	private:
	QGraphicsProxyWidget *m_label;
};

}  // namespace pgeditor::gui
