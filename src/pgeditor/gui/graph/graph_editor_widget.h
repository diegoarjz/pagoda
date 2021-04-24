#pragma once

#include <QGraphicsView>
#include <QWidget>

namespace pagoda
{
class Pagoda;

namespace graph
{
class Graph;
}
}  // namespace pagoda

namespace pgeditor::gui::graph
{
class GraphScene;

class GraphEditorWidget : public QWidget
{
	public:
	GraphEditorWidget(pagoda::Pagoda* pagoda);

	void SetGraph(std::shared_ptr<pagoda::graph::Graph> graph);
	std::shared_ptr<pagoda::graph::Graph> GetGraph() const;

	private:
	void InitializeGUI();
	QWidget* createGraphArea();

	QGraphicsView* m_graphicsView;
	GraphScene* m_graphScene{nullptr};
	std::shared_ptr<pagoda::graph::Graph> m_graph;
	pagoda::Pagoda* m_pagoda;
};  // class GraphEditorWidget
}  // namespace pgeditor::gui::graph
