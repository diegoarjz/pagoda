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
class GraphNode;
class GraphScene : public QGraphicsScene
{
	public:
	GraphScene(pagoda::Pagoda *pagoda);

	void SetProceduralGraph(std::shared_ptr<pagoda::graph::Graph> graph);

	void LayoutGraph();

	void keyPressEvent(QKeyEvent *keyEvent) override;

	private:
	GraphNode *createOperation(const QString &opName);

	std::shared_ptr<pagoda::graph::Graph> m_graph;
	std::unordered_map<pagoda::graph::Node *, GraphNode *> m_operationNodes;
	pagoda::Pagoda *m_pagoda;
};  // class GraphScene
}  // namespace pgeditor::gui
