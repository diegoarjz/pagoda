#include "graph_node.h"

#include "graph_in_port.h"
#include "graph_out_port.h"
#include "node_style.h"

#include <pagoda/graph/query/type.h>

#include <pagoda/graph/graph.h>
#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/node.h>
#include <pagoda/graph/operation_node.h>
#include <pagoda/graph/output_interface_node.h>

#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QStyleOptionGraphicsItem>
#include <QVariant>

using namespace pagoda::graph;
using namespace pagoda::graph::query;

namespace pgeditor::gui
{
GraphNode::GraphNode(NodePtr node, GraphPtr graph) : m_node{node}, m_graph{graph}
{
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);

	InitializeGUI();
}

void GraphNode::InitializeGUI()
{
	using namespace node_style::header;
	using namespace node_style::node;

	m_headerLabel = new QGraphicsProxyWidget(this);

	QFont labelFont(name_font_family, name_font_size);
	QLabel *label = new QLabel(m_node->GetName().c_str());
	label->setFont(labelFont);
	label->setStyleSheet(label_stylesheet);
	m_headerLabel->setWidget(label);
	m_headerLabel->setPos(QPointF(0, 0));

	m_inNodes = m_graph->GetNodeInputNodes(m_node->GetName());
	m_outNodes = m_graph->GetNodeOutputNodes(m_node->GetName());

	NodeSet inputInterfaces;
	auto inputFilter = pagoda::graph::query::type<InputInterfaceNode>(*m_graph, inputInterfaces);
	m_inNodes.ExecuteQuery(inputFilter);

	for (auto n : inputInterfaces) {
		auto interface = std::dynamic_pointer_cast<InputInterfaceNode>(n);
		auto proxy = new GraphInPort(interface, this);
		m_inInterfaces.emplace(n, proxy);
	}

	NodeSet outputInterfaces;
	auto outputFilter = pagoda::graph::query::type<OutputInterfaceNode>(*m_graph, outputInterfaces);
	m_outNodes.ExecuteQuery(outputFilter);
	for (auto n : outputInterfaces) {
		auto interface = std::dynamic_pointer_cast<OutputInterfaceNode>(n);
		auto proxy = new GraphOutPort(interface, this);

		m_outInterfaces.emplace(n, proxy);
	}

	QRectF bounds = boundingRect();
	auto horizontalCenter = bounds.width() * 0.5;
	QRectF headerBounds = m_headerLabel->boundingRect();
	m_headerLabel->setPos(QPointF(horizontalCenter - headerBounds.width() * 0.5, 0));

	auto startYPos = headerBounds.bottom() + ports_top_margin;
	for (auto inPort : m_inInterfaces) {
		inPort.second->setPos(ports_side_margin, startYPos);
		startYPos += inPort.second->boundingRect().height() + port_vertical_spacing;
	}

	startYPos = headerBounds.bottom() + ports_top_margin;
	for (auto outPort : m_outInterfaces) {
		outPort.second->setPos(bounds.width() - outPort.second->boundingRect().width() - ports_side_margin, startYPos);
		startYPos += outPort.second->boundingRect().height() + port_vertical_spacing;
	}
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	using namespace node_style::node;

	QRectF bounds = boundingRect();

	painter->setRenderHint(QPainter::Antialiasing);
	painter->setPen(QPen(border_color, border_width));

	QPainterPath path;
	path.addRoundedRect(bounds, round_size, round_size);

	QLinearGradient gradient(0, 0, 0, bounds.height());
	if (option->state & QStyle::State_Selected) {
		gradient.setColorAt(0, selected_fill_gradient_start);
		gradient.setColorAt(1, selected_fill_gradient_end);
	} else {
		gradient.setColorAt(0, fill_gradient_start);
		gradient.setColorAt(1, fill_gradient_end);
	}

	painter->fillPath(path, gradient);
	painter->drawPath(path);
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
	for (auto inPort : m_inInterfaces) {
		auto bounds = inPort.second->boundingRect();
		if (bounds.width() > maxInPortsWidth) {
			maxInPortsWidth = bounds.width();
		}
		inPortsHeight += bounds.height() + port_vertical_spacing;
	}

	uint32_t maxOutPortsWidth = 0;
	uint32_t outPortsHeight = 0;
	for (auto outPort : m_outInterfaces) {
		auto bounds = outPort.second->boundingRect();
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

GraphInPort *GraphNode::GetInPort(NodePtr node) const
{
	auto iter = m_inInterfaces.find(node);
	if (iter == m_inInterfaces.end()) {
		return nullptr;
	}
	return iter->second;
}

GraphOutPort *GraphNode::GetOutPort(NodePtr node) const
{
	auto iter = m_outInterfaces.find(node);
	if (iter == m_outInterfaces.end()) {
		return nullptr;
	}
	return iter->second;
}

QVariant GraphNode::itemChange(QGraphicsWidget::GraphicsItemChange change, const QVariant &value)
{
	return QGraphicsWidget::itemChange(change, value);
}
}  // namespace pgeditor::gui
