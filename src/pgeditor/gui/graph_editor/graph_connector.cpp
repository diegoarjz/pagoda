
#include "graph_connector.h"

#include "graph_port_connection.h"
#include "node_style.h"

#include <QPainter>

namespace pgeditor::gui
{
GraphConnector::GraphConnector(GraphPortConnection *out, GraphPortConnection *in) : m_out(out), m_in(in)
{
	connect(m_out, &GraphPortConnection::PositionChanged, this, &GraphConnector::PortConnectionChanged);
	connect(m_in, &GraphPortConnection::PositionChanged, this, &GraphConnector::PortConnectionChanged);
}

void GraphConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	using namespace node_style::connector;

	painter->setRenderHint(QPainter::Antialiasing);

	QPainterPath path = shape();
	painter->setPen(QPen(line_color, line_width, Qt::SolidLine));
	painter->drawPath(path);
}

QRectF GraphConnector::boundingRect() const
{
	QRectF inBoundingRect = mapRectFromScene(m_in->mapRectToScene(m_in->boundingRect()));
	QRectF outBoundingRect = mapRectFromScene(m_out->mapRectToScene(m_out->boundingRect()));

	// TODO: this method of adding margins is just a hack to prevent incorrect redrawings.
	return inBoundingRect.united(outBoundingRect).marginsAdded(QMarginsF(100, 100, 100, 100));
}

QPainterPath GraphConnector::shape() const
{
	using namespace node_style::connector;

	QPointF controlPoint1 = mapFromScene(m_out->mapToScene(m_out->boundingRect().center()));
	QPointF controlPoint2(controlPoint1.x() + control_point_distance, controlPoint1.y());
	QPointF controlPoint4 = mapFromScene(m_in->mapToScene(m_in->boundingRect().center()));
	QPointF controlPoint3(controlPoint4.x() - control_point_distance, controlPoint4.y());

	QPainterPath path(controlPoint1);
	path.cubicTo(controlPoint2, controlPoint3, controlPoint4);

	return path;
}

void GraphConnector::PortConnectionChanged(GraphPortConnection *connection, const QPointF &newPos)
{
	this->update(boundingRect());
}
}  // namespace pgeditor::gui
