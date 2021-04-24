#pragma once

#include <QGraphicsScene>

#include <unordered_map>
#include <unordered_set>

namespace pagoda
{
class Pagoda;

namespace graph
{
class Graph;
class Node;
}  // namespace graph
}  // namespace pagoda

namespace pgeditor::gui::graph
{
class NewConnectionEdge;
class GraphNode;
class GraphPort;
class GraphConnector;

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
	/**
	 * Creates an operation node with the given op name.
	 */
	GraphNode *createOperation(const QString &opName);

	/**
	 * Deletes the given graph operation node
	 */
	void deleteOperationNode(GraphNode *operationNode);

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
	std::unordered_map<GraphNode *, std::unordered_set<GraphConnector *>> m_connectorsByNode;
	pagoda::Pagoda *m_pagoda;
};  // class GraphScene
}  // namespace pgeditor::gui::graph
