#include "graph_port_connection.h"

#include "node_style.h"

#include <QPainter>
#include <QtDebug>

namespace pgeditor::gui::graph
{
GraphPortConnection::~GraphPortConnection() {}

GraphPortConnection::GraphPortConnection(QGraphicsItem *parent, GraphNode *node, NodeSide side)
  : QGraphicsWidget(parent), m_node{node}, m_nodeSide{side}
{
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

GraphNode *GraphPortConnection::GetNode() const { return m_node; }
GraphPortConnection::NodeSide GraphPortConnection::GetNodeSide() const { return m_nodeSide; }

void GraphPortConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	using namespace node_style::port_connection;

	QRectF bounds = boundingRect();

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(border_color, border_width));

	QPainterPath path;
	path.addEllipse(bounds);

	painter->fillPath(path, QBrush(fill_color));
	painter->drawPath(path);
}

QRectF GraphPortConnection::boundingRect() const
{
	using namespace node_style::port_connection;
	return QRectF(0, 0, port_size, port_size);
}

QPainterPath GraphPortConnection::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	return path;
}

QVariant GraphPortConnection::itemChange(QGraphicsWidget::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsWidget::ItemScenePositionHasChanged) {
		emit PositionChanged(this, mapToScene(pos()));
	}

	return QGraphicsWidget::itemChange(change, value);
}
}  // namespace pgeditor::gui::graph
