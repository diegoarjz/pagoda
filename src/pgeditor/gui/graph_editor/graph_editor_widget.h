#pragma once

#include <QGraphicsView>
#include <QWidget>

namespace pagoda::graph
{
class Graph;
}

namespace pgeditor::gui
{
class GraphScene;

class GraphEditorWidget : public QWidget
{
	public:
	GraphEditorWidget();

	void SetGraph(std::shared_ptr<pagoda::graph::Graph> graph);

	private:
	void InitializeGUI();
	QWidget* createGraphArea();

	QGraphicsView* m_graphicsView;
	GraphScene* m_graphScene{nullptr};
};  // class GraphEditorWidget
}  // namespace pgeditor::gui
