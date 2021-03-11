
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
GraphEditorWidget::GraphEditorWidget(pagoda::Pagoda *pagoda) : QWidget(), m_pagoda{pagoda} { InitializeGUI(); }

void GraphEditorWidget::InitializeGUI()
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->QLayout::setSpacing(0);

	setLayout(layout);

	auto toolbar = new QToolBar;
	toolbar->setContentsMargins(-1, -1, -1, 0);
	toolbar->addAction(tr("Layout"), [this]() {
		if (m_graphScene != nullptr) {
			m_graphScene->LayoutGraph();
		}
	});

	layout->addWidget(toolbar);
	layout->addWidget(createGraphArea());
}

QWidget *GraphEditorWidget::createGraphArea()
{
	m_graphicsView = new QGraphicsView();
	m_graphicsView->setContentsMargins(-1, -1, -1, 0);

	return m_graphicsView;
}

void GraphEditorWidget::SetGraph(std::shared_ptr<Graph> graph)
{
	m_graphScene = new GraphScene(m_pagoda);
	m_graphScene->SetProceduralGraph(graph);
	m_graphicsView->setScene(m_graphScene);
	m_graph = graph;
}

std::shared_ptr<Graph> GraphEditorWidget::GetGraph() const { return m_graph; }
}  // namespace pgeditor::gui
