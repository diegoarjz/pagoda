#include "edge.h"

#include "node_style.h"

#include <QPainter>

namespace pgeditor::gui::graph
{
Edge::Edge(const QPointF &start, const QPointF &end) : m_start{start}, m_end{end} {}

Edge::~Edge() {}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	using namespace node_style::connector;

	painter->setRenderHint(QPainter::Antialiasing);

	QPainterPath path = shape();
	painter->setPen(QPen(line_color, line_width, Qt::SolidLine));
	painter->drawPath(path);
}

QRectF Edge::boundingRect() const
{
	QRectF bounds{QPointF{std::min(m_start.x(), m_end.x()), std::min(m_start.y(), m_end.y())},
	              QPointF{std::max(m_start.x(), m_end.x()), std::max(m_start.y(), m_end.y())}};

	return bounds.marginsAdded({100, 100, 100, 100});
}

QPainterPath Edge::shape() const
{
	using namespace node_style::connector;

	QPointF controlPoint1 = m_start;
	QPointF controlPoint2(controlPoint1.x() + control_point_distance, controlPoint1.y());
	QPointF controlPoint4 = m_end;
	QPointF controlPoint3(controlPoint4.x() - control_point_distance, controlPoint4.y());

	QPainterPath path(controlPoint1);
	path.cubicTo(controlPoint2, controlPoint3, controlPoint4);

	return path;
}
}  // namespace pgeditor::gui::graph
