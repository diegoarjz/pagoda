#pragma once

#include <QGraphicsWidget>

#include <memory>

namespace pagoda::graph
{
class Node;
using NodePtr = std::shared_ptr<Node>;
}  // namespace pagoda::graph

namespace pgeditor::gui
{
class GraphNode;
class GraphPortConnection;
class NewConnectionEdge;

class GraphPort : public QGraphicsWidget
{
	Q_OBJECT

	public:
	GraphPort(GraphNode *node, QGraphicsWidget *parent);
	~GraphPort() override {}

	pagoda::graph::NodePtr GetNode() const;
	GraphPortConnection *GetPortConnection() const;

	signals:
	/**
	 * Notifies that the user started to drag from this port.
	 */
	void StartConnectionDrag(GraphPort *port, NewConnectionEdge *newEdge);
	/**
	 * Signals that a new connection has been made between the ports.
	 */
	void NewNodeConnection(GraphPort *from, GraphPort *to);

	protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
	void dragEnterEvent(QGraphicsSceneDragDropEvent *e) override;
	void dragMoveEvent(QGraphicsSceneDragDropEvent *e) override;
	void dropEvent(QGraphicsSceneDragDropEvent *e) override;

	GraphNode *m_node;
	GraphPortConnection *m_connection;
};

}  // namespace pgeditor::gui
