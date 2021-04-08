#pragma once

#include <QGraphicsScene>

#include <unordered_map>

namespace pagoda
{
class Pagoda;

namespace graph
{
class Graph;
class Node;
}  // namespace graph
}  // namespace pagoda

namespace pgeditor::gui
{
class NewConnectionEdge;
class GraphNode;
class GraphPort;

class GraphScene : public QGraphicsScene
{
	public:
	GraphScene(pagoda::Pagoda *pagoda);

	void SetProceduralGraph(std::shared_ptr<pagoda::graph::Graph> graph);

	void LayoutGraph();

	protected:
	void keyPressEvent(QKeyEvent *keyEvent) override;
	void dragMoveEvent(QGraphicsSceneDragDropEvent *e) override;

	private:
	GraphNode *createOperation(const QString &opName);

	/**
	 * Starts a graph edge anchored to port.
	 */
	void StartConnectionDrag(GraphPort *port, NewConnectionEdge *newEdge);

	/**
	 * Creates a connection between two nodes.
	 */
	void ConnectedNodes(GraphPort *from, GraphPort *to);

	NewConnectionEdge *m_newEdge;

	std::shared_ptr<pagoda::graph::Graph> m_graph;
	std::unordered_map<pagoda::graph::Node *, GraphNode *> m_operationNodes;
	pagoda::Pagoda *m_pagoda;
};  // class GraphScene
}  // namespace pgeditor::gui
