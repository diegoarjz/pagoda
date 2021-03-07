#include "main_window.h"

#include "graph_editor/graph_editor_widget.h"

#include <pagoda/graph/io/reader.h>

#include <QDockWidget>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>

namespace pgeditor::gui
{
MainWindow::MainWindow()
{
	using namespace pagoda::graph::io;

	InitializeGUI();
	InitializeMenus();
}

MainWindow::~MainWindow() {}

void MainWindow::InitializeGUI()
{
	QWidget *sceneViewer = new QWidget();
	m_graphEditor = new GraphEditorWidget();
	QWidget *inspector = new QWidget();

	this->setCentralWidget(sceneViewer);

	QDockWidget *graphEditorWidget = new QDockWidget(QString("Graph Editor"), this);
	this->addDockWidget(Qt::BottomDockWidgetArea, graphEditorWidget);
	graphEditorWidget->setFeatures(graphEditorWidget->features() & ~QDockWidget::DockWidgetClosable &
	                               ~QDockWidget::DockWidgetFloatable);
	graphEditorWidget->setWidget(m_graphEditor);
	graphEditorWidget->show();

	QDockWidget *inspectorWidget = new QDockWidget(QString("Inspector"), this);
	inspectorWidget->setFeatures(inspectorWidget->features() & ~QDockWidget::DockWidgetClosable &
	                             ~QDockWidget::DockWidgetFloatable);
	inspectorWidget->setWidget(inspector);
	this->addDockWidget(Qt::RightDockWidgetArea, inspectorWidget);
}

void MainWindow::InitializeMenus()
{
	QMenu *fileMenu = menuBar()->addMenu(QString("File"));
	QAction *openGraphAction = new QAction(QString("Open"), this);
	fileMenu->addAction(openGraphAction);

	connect(openGraphAction, &QAction::triggered, this, &MainWindow::OpenGraph);
}

// Slots
void MainWindow::OpenGraph()
{
	using namespace pagoda::graph::io;

	QString fileName =
	  QFileDialog::getOpenFileName(this, tr("Open Procedural Graph"), "", tr("Procedural Graph (*.pgd);;All Files (*)"));

	if (fileName.isEmpty()) {
		return;
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(this, tr("Unable to open file"), file.errorString());
		return;
	}

	GraphReader reader(m_pagoda.GetNodeFactory());
	auto graph = reader.Read(file.readAll().constData());

	if (graph == nullptr || reader.GetParseResult().status != ParseResult::Status::Ok) {
		QMessageBox::information(this, tr("Error parsing graph."), "Error parsing graph.");
		return;
	}

	m_graphEditor->SetGraph(graph);
}

}  // namespace pgeditor::gui
