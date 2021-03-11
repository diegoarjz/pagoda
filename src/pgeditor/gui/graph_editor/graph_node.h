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

namespace pgeditor::gui
{
class GraphInPort;
class GraphOutPort;

class GraphNode : public QGraphicsWidget
{
	Q_OBJECT
	public:
	GraphNode(pagoda::graph::NodePtr node, pagoda::graph::GraphPtr graph);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;

	GraphInPort *GetInPort(pagoda::graph::NodePtr node) const;
	GraphOutPort *GetOutPort(pagoda::graph::NodePtr node) const;

	/**
	 * Sets the node position.
	 * Updates the \c GraphNode GUI position and the \c Node's position
	 * parameters.
	 */
	void SetPosition(int32_t x, int32_t y);

	protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	private:
	void InitializeGUI();

	pagoda::graph::NodePtr m_node;
	pagoda::graph::GraphPtr m_graph;

	pagoda::graph::NodeSet m_inNodes;
	pagoda::graph::NodeSet m_outNodes;

	std::unordered_map<pagoda::graph::NodePtr, GraphInPort *> m_inInterfaces;
	std::unordered_map<pagoda::graph::NodePtr, GraphOutPort *> m_outInterfaces;

	QGraphicsProxyWidget *m_headerLabel;
};  // class GraphNode
}  // namespace pgeditor::gui
