#pragma once

#include <pagoda/graph/node_set.h>

#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsWidget>
#include <QLabel>
#include <QPainter>

#include <memory>
#include <unordered_map>

namespace pagoda::graph
{
class Graph;
using GraphPtr = std::shared_ptr<Graph>;
class Node;
using NodePtr = std::shared_ptr<Node>;
}  // namespace pagoda::graph

namespace pgeditor::gui::graph
{
class GraphPort;
class GraphInPort;
class GraphOutPort;

class GraphNode : public QGraphicsWidget
{
	Q_OBJECT

	public:
	GraphNode(pagoda::graph::GraphPtr graph, pagoda::graph::NodePtr node);
	~GraphNode() override;

	void InitializeGUI();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	/**
	 * Returns the \c Node.
	 */
	pagoda::graph::NodePtr GetNode() const;

	void ForEachPort(std::function<void(GraphPort *)> f);

	/**
	 * Sets the node position.
	 * Updates the \c GraphNode GUI position and the \c Node's position
	 * parameters.
	 */
	void SetPosition(int32_t x, int32_t y);

	/**
	 * Connects the two ports.
	 *
	 * Emits NewConnection signal.
	 */
	void ConnectInterfaces(GraphPort *from, GraphPort *to);

	signals:

	/**
	 * This node has been connected to another node.
	 */
	void NewConnection(GraphPort *from, GraphPort *to);

	protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	virtual void CreateBody() = 0;
	void AddPort(GraphInPort *port);
	void AddPort(GraphOutPort *port);

	pagoda::graph::NodePtr m_node;
	pagoda::graph::GraphPtr m_graph;

	QGraphicsProxyWidget *m_headerLabel;

	std::vector<GraphInPort *> m_inPorts;
	std::vector<GraphOutPort *> m_outPorts;
	bool m_guiInitialized{false};
};  // class GraphNode
}  // namespace pgeditor::gui::graph
