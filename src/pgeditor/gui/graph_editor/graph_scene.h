#pragma once

#include <QGraphicsScene>

#include <unordered_map>

namespace pagoda::graph
{
class Graph;
class Node;
}  // namespace pagoda::graph

namespace pgeditor::gui
{
class GraphNode;
class GraphScene : public QGraphicsScene
{
	public:
	GraphScene();

	void SetProceduralGraph(std::shared_ptr<pagoda::graph::Graph> graph);

	private:
	std::shared_ptr<pagoda::graph::Graph> m_graph;
	std::unordered_map<pagoda::graph::Node *, GraphNode *> m_operationNodes;
};  // class GraphScene
}  // namespace pgeditor::gui
