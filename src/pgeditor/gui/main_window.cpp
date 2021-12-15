#include "main_window.h"

#include "graph/graph_editor_widget.h"

#include <pgeditor/gui/viewer/metal_viewer.h>
#include <pgeditor/gui/viewer/viewer.h>
#include <pgeditor/renderer/rendering_system.h>

#include <pagoda/graph/io/reader.h>
#include <pagoda/graph/io/writer.h>

#include <QDockWidget>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QSurfaceFormat>

#include <sstream>

using namespace pgeditor::gui::viewer;

namespace pgeditor::gui
{
MainWindow::MainWindow()
{
	using namespace pagoda::graph::io;

	// Register the rendering system with pagoda
	renderer::RenderingSystem::Registration(&m_pagoda);

	// InitializeGL();
	InitializeGUI();
	InitializeMenus();

	m_graphEditor->SetGraph(m_pagoda.CreateGraph());
}

MainWindow::~MainWindow() {}

void MainWindow::InitializeGL()
{
	QSurfaceFormat format;
	format.setMajorVersion(4);

#if !defined(Q_OS_MAC)
	format.setMinorVersion(1);
#else
	format.setMinorVersion(2);
#endif
	format.setDepthBufferSize(24);
	format.setSamples(4);
	format.setProfile(QSurfaceFormat::CoreProfile);

	QSurfaceFormat::setDefaultFormat(format);
}

void MainWindow::InitializeGUI()
{
	m_graphEditor = new graph::GraphEditorWidget(&m_pagoda);
	// QWidget *inspector = new QWidget();

	QDockWidget *graphEditorWidget = new QDockWidget(QString("Graph Editor"), this);
	addDockWidget(Qt::BottomDockWidgetArea, graphEditorWidget);
	graphEditorWidget->setFeatures(graphEditorWidget->features() & ~QDockWidget::DockWidgetClosable &
	                               ~QDockWidget::DockWidgetFloatable);
	graphEditorWidget->setWidget(m_graphEditor);
	graphEditorWidget->show();
	graphEditorWidget->setTitleBarWidget(new QWidget(graphEditorWidget));

	/*
	QDockWidget *inspectorWidget = new QDockWidget(QString("Inspector"), this);
	inspectorWidget->setFeatures(inspectorWidget->features() & ~QDockWidget::DockWidgetClosable &
	                             ~QDockWidget::DockWidgetFloatable);
	inspectorWidget->setWidget(inspector);
	addDockWidget(Qt::RightDockWidgetArea, inspectorWidget);
	*/

	/*
	m_openGLWidget = new Viewer(this);
	m_openGLWidget->setFormat(QSurfaceFormat::defaultFormat());
	m_openGLWidget->SetRenderingSystem(
	  m_pagoda.GetProceduralObjectSystem()->GetComponentSystem<renderer::RenderingSystem>());
	setCentralWidget(m_openGLWidget);
	*/

	m_metalViewer = new MetalViewer(this);
	setCentralWidget(m_metalViewer);
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

void MainWindow::LoadGraphFile(const std::string &filePath)
{
	using namespace pagoda::graph::io;

	QFile file(filePath.c_str());
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

// Slots
void MainWindow::OpenGraph()
{
	using namespace pagoda::graph::io;

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Graph"), "", tr("Graph Files (*.pgd);;All Files (*)"));

	if (fileName.isEmpty()) {
		return;
	}
	LoadGraphFile(fileName.toStdString());
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
