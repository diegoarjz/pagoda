#include "graph_node.h"

#include "graph_in_port.h"
#include "graph_out_port.h"
#include "node_style.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/node.h>

#include <pagoda/dynamic/integer_value.h>

#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QStyleOptionGraphicsItem>
#include <QVariant>

#include <iostream>

using namespace pagoda::graph;
using namespace pagoda::graph::query;
using namespace pagoda::dynamic;

namespace pgeditor::gui::graph
{
GraphNode::GraphNode(GraphPtr graph, NodePtr node) : m_node{node}, m_graph{graph}
{
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
}

GraphNode::~GraphNode() {}

void GraphNode::InitializeGUI()
{
	if (m_guiInitialized) {
		return;
	}
	m_guiInitialized = true;
	using namespace node_style::header;
	using namespace node_style::node;

	QFont labelFont(name_font_family, name_font_size);
	QLabel *label = new QLabel(m_node->GetName().c_str());
	label->setFont(labelFont);
	label->setStyleSheet(label_stylesheet);
	m_headerLabel = new QGraphicsProxyWidget(this);
	m_headerLabel->setWidget(label);
	m_headerLabel->setPos(QPointF(0, 0));

	CreateBody();
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	using namespace node_style::node;

	QRectF bounds = boundingRect();

	QRectF headerBounds = m_headerLabel->boundingRect();
	headerBounds.setWidth(bounds.width());
	headerBounds.setHeight(headerBounds.height() + 2);
	bounds.setTop(bounds.top() + headerBounds.height());

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(border_color, border_width));

	if (option->state & QStyle::State_Selected) {
		painter->fillRect(bounds, selected_fill_gradient_start);
	} else {
		painter->fillRect(bounds, fill_gradient_start);
	}
	painter->drawRect(bounds);

	painter->fillRect(headerBounds, QColor(1, 1, 1, 1));
	painter->drawRect(headerBounds);
}

QPainterPath GraphNode::shape() const
{
	using namespace node_style::node;

	QRectF bounds = boundingRect();
	QPainterPath path;
	path.addRoundedRect(bounds, round_size, round_size);

	return path;
}

QRectF GraphNode::boundingRect() const
{
	using namespace node_style::node;

	auto headerBounds = m_headerLabel->boundingRect();
	uint32_t height = headerBounds.height();
	uint32_t width = headerBounds.width();

	uint32_t maxInPortsWidth = 0;
	uint32_t inPortsHeight = 0;
	for (auto inPort : m_inPorts) {
		auto bounds = inPort->boundingRect();
		if (bounds.width() > maxInPortsWidth) {
			maxInPortsWidth = bounds.width();
		}
		inPortsHeight += bounds.height() + port_vertical_spacing;
	}

	uint32_t maxOutPortsWidth = 0;
	uint32_t outPortsHeight = 0;
	for (auto outPort : m_outPorts) {
		auto bounds = outPort->boundingRect();
		if (bounds.width() > maxOutPortsWidth) {
			maxOutPortsWidth = bounds.width();
		}
		outPortsHeight += bounds.height() + port_vertical_spacing;
	}

	height += std::max(inPortsHeight, outPortsHeight) + ports_top_margin + ports_bottom_margin;
	width = std::max(static_cast<uint32_t>(m_headerLabel->boundingRect().width()), maxInPortsWidth + maxOutPortsWidth) +
	        2 * ports_side_margin + port_horizontal_spacing;

	return QRectF(0, 0, width, height);
}

pagoda::graph::NodePtr GraphNode::GetNode() const { return m_node; }

void GraphNode::ForEachPort(std::function<void(GraphPort *)> f)
{
	for (auto &p : m_inPorts) {
		f(p);
	}

	for (auto &p : m_outPorts) {
		f(p);
	}
}

void GraphNode::ConnectInterfaces(GraphPort *from, GraphPort *to)
{
	DBG_ASSERT(std::dynamic_pointer_cast<InputInterfaceNode>(from->GetNode()) != nullptr &&
	           std::dynamic_pointer_cast<OutputInterfaceNode>(to->GetNode()) != nullptr);
	m_graph->CreateEdge(from->GetNode()->GetName(), to->GetNode()->GetName());
	emit NewConnection(from, to);
}

void GraphNode::AddPort(GraphInPort *port)
{
	m_inPorts.push_back(port);
	connect(port, &GraphPort::NewNodeConnection, this, &GraphNode::ConnectInterfaces);
}

void GraphNode::AddPort(GraphOutPort *port)
{
	m_outPorts.push_back(port);
	connect(port, &GraphPort::NewNodeConnection, this, &GraphNode::ConnectInterfaces);
}

void GraphNode::SetPosition(int32_t x, int32_t y)
{
	m_node->RegisterOrSetMember("posX", std::make_shared<Integer>(x));
	m_node->RegisterOrSetMember("posY", std::make_shared<Integer>(y));
	setPos(x, y);
}

QVariant GraphNode::itemChange(QGraphicsWidget::GraphicsItemChange change, const QVariant &value)
{
	if (change == QGraphicsWidget::GraphicsItemChange::ItemPositionChange) {
		auto position = pos();
		m_node->RegisterOrSetMember("posX", std::make_shared<Integer>(static_cast<int>(position.x())));
		m_node->RegisterOrSetMember("posY", std::make_shared<Integer>(static_cast<int>(position.y())));
	}
	return QGraphicsWidget::itemChange(change, value);
}
}  // namespace pgeditor::gui::graph
