#include "new_connection_edge.h"

#include "graph_port_connection.h"

#include <QRect>

#include <iostream>

namespace pgeditor::gui::graph
{
NewConnectionEdge::NewConnectionEdge(GraphPortConnection* port)
  : Edge{port->boundingRect().center(), port->boundingRect().center()}, m_port{port}
{
}

NewConnectionEdge::~NewConnectionEdge() {}

void NewConnectionEdge::CursorPositionChanged(const QPointF& pos)
{
	auto center = m_port->mapToScene(m_port->boundingRect().center());
	if (m_port->GetNodeSide() == GraphPortConnection::NodeSide::Left) {
		m_start = pos;
		m_end = center;
	} else {
		m_end = pos;
		m_start = center;
	}
	this->prepareGeometryChange();
	this->update(boundingRect());
}
}  // namespace pgeditor::gui::graph
