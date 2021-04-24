#include "graph_out_port.h"

#include "graph_node.h"
#include "graph_port_connection.h"

#include <pagoda/graph/node.h>
#include <pagoda/graph/output_interface_node.h>

#include <QFont>
#include <QLabel>

#include "node_style.h"

using namespace pagoda::graph;

namespace pgeditor::gui::graph
{
GraphOutPort::GraphOutPort(GraphNode *node, const pagoda::graph::OutputInterfaceNodePtr &interface,
                           QGraphicsWidget *parent)
  : GraphPort(node, parent), m_interface{interface}
{
	using namespace node_style::port;

	m_connection = new GraphPortConnection(this, node, GraphPortConnection::NodeSide::Right);
	QFont labelFont(name_font_family, name_font_size);

	m_label = new QGraphicsProxyWidget(this);
	QLabel *label = new QLabel(m_interface->GetInterfaceName().c_str());
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

}  // namespace pgeditor::gui::graph
