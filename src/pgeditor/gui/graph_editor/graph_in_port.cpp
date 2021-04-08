#include "graph_in_port.h"

#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>

#include <QLabel>

#include "graph_port_connection.h"
#include "node_style.h"

using namespace pagoda::graph;

namespace pgeditor::gui
{
GraphInPort::GraphInPort(pagoda::graph::NodePtr node, QGraphicsWidget *parent) : GraphPort(node, parent)
{
	using namespace node_style::port;

	m_connection = new GraphPortConnection(this, GraphPortConnection::NodeSide::Left);
	QFont labelFont(name_font_family, name_font_size);

	auto inputInterface = std::dynamic_pointer_cast<InputInterfaceNode>(node);

	m_label = new QGraphicsProxyWidget(this);
	QLabel *label = new QLabel(inputInterface->GetInterfaceName().c_str());
	label->setStyleSheet(label_stylesheet);
	label->setFont(labelFont);
	m_label->setWidget(label);
	m_label->setPos(QPointF(m_connection->boundingRect().width() + horizontal_spacing, 0));

	auto verticalCenter = m_label->boundingRect().height() * 0.5;
	QRectF connectionBounds = m_connection->boundingRect();

	m_connection->setPos(QPointF(0, verticalCenter - connectionBounds.height() * 0.5));
}

QRectF GraphInPort::boundingRect() const
{
	using namespace node_style::port;

	QRectF labelBounds = m_label->boundingRect();
	QRectF connectionBounds = m_connection->boundingRect();
	auto width = labelBounds.width() + connectionBounds.width() + horizontal_spacing;
	auto height = std::max(labelBounds.height(), connectionBounds.height());
	return QRectF(0, 0, width, height);
}

}  // namespace pgeditor::gui
