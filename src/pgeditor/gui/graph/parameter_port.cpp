#include "parameter_port.h"

#include <QLabel>

#include "graph_node.h"
#include "graph_port_connection.h"
#include "node_style.h"

namespace pgeditor::gui::graph
{
ParameterPort::ParameterPort(GraphNode *node, const std::string &paramName, QGraphicsWidget *parent)
  : GraphPort(node, parent), m_paramName{paramName}
{
	using namespace node_style::port;

	m_connection = new GraphPortConnection(this, node, GraphPortConnection::NodeSide::Left);
	QFont labelFont(name_font_family, name_font_size);

	m_label = new QGraphicsProxyWidget(this);
	QLabel *label = new QLabel(m_paramName.c_str());
	label->setStyleSheet(label_stylesheet);
	label->setFont(labelFont);
	m_label->setWidget(label);
	m_label->setPos(QPointF(m_connection->boundingRect().width() + horizontal_spacing, 0));

	auto verticalCenter = m_label->boundingRect().height() * 0.5;
	QRectF connectionBounds = m_connection->boundingRect();

	m_connection->setPos(QPointF(0, verticalCenter - connectionBounds.height() * 0.5));
}

QRectF ParameterPort::boundingRect() const
{
	using namespace node_style::port;

	QRectF labelBounds = m_label->boundingRect();
	QRectF connectionBounds = m_connection->boundingRect();
	auto width = labelBounds.width() + connectionBounds.width() + horizontal_spacing;
	auto height = std::max(labelBounds.height(), connectionBounds.height());
	return QRectF(0, 0, width, height);
}
}  // namespace pgeditor::gui::graph
