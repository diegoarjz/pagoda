#pragma once

#include <QGraphicsWidget>

namespace pgeditor::gui
{
class GraphPortConnection;

class GraphConnector : public QGraphicsWidget
{
	public:
	GraphConnector(GraphPortConnection *out, GraphPortConnection *in);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	public slots:
	void PortConnectionChanged(GraphPortConnection *connection, const QPointF &newPos);

	private:
	GraphPortConnection *m_out;
	GraphPortConnection *m_in;
};  // class GraphConnector
}  // namespace pgeditor::gui
