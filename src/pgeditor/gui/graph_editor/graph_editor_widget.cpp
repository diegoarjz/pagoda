
#include "graph_editor_widget.h"

#include "graph_scene.h"

#include <QGraphicsView>
#include <QLabel>
#include <QScrollArea>
#include <QSplitter>
#include <QToolBar>
#include <QVBoxLayout>
#include <QVariant>
#include <QWidget>

using namespace pagoda::graph;

namespace pgeditor::gui
{
GraphEditorWidget::GraphEditorWidget() { InitializeGUI(); }

void GraphEditorWidget::InitializeGUI()
{
	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	auto toolbar = new QToolBar;
	toolbar->addAction(tr("Layout"), [this]() {
		if (m_graphScene != nullptr) {
			m_graphScene->LayoutGraph();
		}
	});

	layout->QLayout::addWidget(toolbar);
	layout->addWidget(createGraphArea());
}

QWidget *GraphEditorWidget::createGraphArea()
{
	m_graphicsView = new QGraphicsView();

	return m_graphicsView;
}

void GraphEditorWidget::SetGraph(std::shared_ptr<Graph> graph)
{
	m_graphScene = new GraphScene();
	m_graphScene->SetProceduralGraph(graph);
	m_graphicsView->setScene(m_graphScene);
}

}  // namespace pgeditor::gui
