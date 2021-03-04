#include "graph_out_port.h"

#include "graph_port_connection.h"

#include <pagoda/graph/node.h>
#include <pagoda/graph/output_interface_node.h>

#include <QFont>
#include <QLabel>

#include "node_style.h"

using namespace pagoda::graph;

namespace pgeditor::gui
{
GraphOutPort::GraphOutPort(pagoda::graph::NodePtr node, QGraphicsWidget *parent) : GraphPort(node, parent)
{
	using namespace node_style::port;

	m_connection = new GraphPortConnection(this);
	QFont labelFont(name_font_family, name_font_size);

	auto outputInterface = std::dynamic_pointer_cast<OutputInterfaceNode>(node);

	m_label = new QGraphicsProxyWidget(this);
	QLabel *label = new QLabel(outputInterface->GetInterfaceName().c_str());
	label->setFont(labelFont);
	label->setStyleSheet(label_stylesheet);
	m_label->setWidget(label);

	auto verticalCenter = m_label->boundingRect().height() * 0.5;
	QRectF connectionBounds = m_connection->boundingRect();

	m_connection->setPos(
	  QPointF(m_label->boundingRect().width() + horizontal_spacing, verticalCenter - connectionBounds.height() * 0.5));
}

QRectF GraphOutPort::boundingRect() const
{
	using namespace node_style::port;

	QRectF labelBounds = m_label->boundingRect();
	QRectF connectionBounds = m_connection->boundingRect();
	auto width = labelBounds.width() + connectionBounds.width() + horizontal_spacing;
	auto height = std::max(labelBounds.height(), connectionBounds.height());
	return QRectF(0, 0, width, height);
}

}  // namespace pgeditor::gui
