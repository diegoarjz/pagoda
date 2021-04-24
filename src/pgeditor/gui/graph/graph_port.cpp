#include "graph_port.h"

#include "graph_node.h"
#include "new_connection_edge.h"

#include <pagoda/graph/input_interface_node.h>
#include <pagoda/graph/output_interface_node.h>

#include <QDrag>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QWidget>
#include <QtDebug>

using namespace pagoda::graph;

namespace pgeditor::gui::graph
{
struct GraphPortMimeData : public QMimeData
{
	enum class PortType
	{
		Input,
		Output
	};

	GraphPortMimeData(GraphPort *interface)
	  : m_port{interface}, m_edge{new NewConnectionEdge{interface->GetPortConnection()}}
	{
		if (std::dynamic_pointer_cast<InputInterfaceNode>(m_port->GetNode()) != nullptr) {
			m_interfaceType = PortType::Input;
		} else {
			m_interfaceType = PortType::Output;
		}
		m_port->scene()->addItem(m_edge.get());
	}

	~GraphPortMimeData() override
	{
		if (m_edge != nullptr) {
			m_port->scene()->removeItem(m_edge.get());
		}
	}

	GraphPort *m_port;
	PortType m_interfaceType;
	std::unique_ptr<NewConnectionEdge> m_edge;
};

GraphPort::GraphPort(GraphNode *node, QGraphicsWidget *parent) : QGraphicsWidget(parent), m_node(node)
{
	setAcceptDrops(true);
}

GraphPortConnection *GraphPort::GetPortConnection() const { return m_connection; }

pagoda::graph::NodePtr GraphPort::GetNode() const { return m_node->GetNode(); }

void GraphPort::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		auto d = new GraphPortMimeData{this};

		emit StartConnectionDrag(this, d->m_edge.get());
		QDrag drag(e->widget());
		drag.setMimeData(d);
		drag.exec();
	} else {
		QGraphicsWidget::mousePressEvent(e);
	}
}

void GraphPort::dragEnterEvent(QGraphicsSceneDragDropEvent *e)
{
	//
	QGraphicsWidget::dragEnterEvent(e);
}

void GraphPort::dragMoveEvent(QGraphicsSceneDragDropEvent *e)
{
	auto mimeData = dynamic_cast<const GraphPortMimeData *>(e->mimeData());
	if (mimeData != nullptr) {
		e->accept();
	}
	QGraphicsWidget::dragMoveEvent(e);
}

void GraphPort::dropEvent(QGraphicsSceneDragDropEvent *e)
{
	auto mimeData = dynamic_cast<const GraphPortMimeData *>(e->mimeData());
	if (mimeData != nullptr) {
		GraphPort *from = nullptr;
		GraphPort *to = nullptr;
		if (mimeData->m_interfaceType == GraphPortMimeData::PortType::Input) {
			from = this;
			to = mimeData->m_port;
		} else {
			from = mimeData->m_port;
			to = this;
		}
		emit NewNodeConnection(from, to);
	}
}
}  // namespace pgeditor::gui::graph
