
#include "graph_connector.h"

#include "graph_port_connection.h"
#include "node_style.h"

#include <QPainter>

namespace pgeditor::gui::graph
{
GraphConnector::GraphConnector(GraphPortConnection *out, GraphPortConnection *in)
  : Edge(in->boundingRect().center(), out->boundingRect().center()), m_out(out), m_in(in)
{
	connect(m_out, &GraphPortConnection::PositionChanged, this, &GraphConnector::PortConnectionChanged);
	connect(m_in, &GraphPortConnection::PositionChanged, this, &GraphConnector::PortConnectionChanged);
}

GraphNode *GraphConnector::GetUpstreamNode() const { return m_out->GetNode(); }

GraphNode *GraphConnector::GetDownstreamNode() const { return m_in->GetNode(); }

void GraphConnector::PortConnectionChanged(GraphPortConnection *connection, const QPointF &newPos)
{
	m_start = mapRectFromScene(m_out->mapRectToScene(m_out->boundingRect())).center();
	m_end = mapRectFromScene(m_in->mapRectToScene(m_in->boundingRect())).center();
	this->prepareGeometryChange();
	this->update(boundingRect());
}
}  // namespace pgeditor::gui::graph
