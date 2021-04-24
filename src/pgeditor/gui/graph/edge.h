#pragma once

#include <QGraphicsWidget>

namespace pgeditor::gui::graph
{
/**
 * Class used to represent a graph edge.
 */
class Edge : public QGraphicsWidget
{
	public:
	Edge(const QPointF &start, const QPointF &end);
	~Edge() override;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	protected:
	QPointF m_start;  ///< Position of the left endpoint
	QPointF m_end;    ///< Position of the right endpoint
};
}  // namespace pgeditor::gui::graph
