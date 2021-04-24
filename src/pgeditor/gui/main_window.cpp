#include "main_window.h"

#include "graph/graph_editor_widget.h"

#include <pagoda/graph/io/reader.h>
#include <pagoda/graph/io/writer.h>

#include <QDockWidget>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>

#include <sstream>

namespace pgeditor::gui
{
MainWindow::MainWindow()
{
	using namespace pagoda::graph::io;

	InitializeGUI();
	InitializeMenus();

	m_graphEditor->SetGraph(m_pagoda.CreateGraph());
}

MainWindow::~MainWindow() {}

void MainWindow::InitializeGUI()
{
	QWidget *sceneViewer = new QWidget();
	m_graphEditor = new graph::GraphEditorWidget(&m_pagoda);
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
	QAction *saveGraphAction = new QAction(QString("Save"), this);
	fileMenu->addAction(openGraphAction);
	fileMenu->addAction(saveGraphAction);

	connect(openGraphAction, &QAction::triggered, this, &MainWindow::OpenGraph);
	connect(saveGraphAction, &QAction::triggered, this, &MainWindow::SaveGraph);
}

// Slots
void MainWindow::OpenGraph()
{
	using namespace pagoda::graph::io;

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Graph"), "", tr("Graph Files (*.pgd);;All Files (*)"));

	if (fileName.isEmpty()) {
		return;
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::information(this, tr("Unable to open file."), file.errorString());
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

void MainWindow::SaveGraph()
{
	using namespace pagoda::graph::io;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Graph"), "", tr("Graph Files (*.pgd);;All Files (*)"));

	if (fileName.isEmpty()) {
		return;
	}

	GraphWriter writer(m_graphEditor->GetGraph());
	std::stringstream outStream;
	writer.Write(outStream);

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("Unable to save file."), file.errorString());
		return;
	}

	std::string fileContents = outStream.str();
	file.write(fileContents.c_str(), fileContents.size());
}
}  // namespace pgeditor::gui
