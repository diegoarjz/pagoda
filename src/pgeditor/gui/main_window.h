#pragma once

#include <pagoda/pagoda.h>

#include <QMainWindow>
#include <QWidget>
#include <QtCore>

namespace pgeditor::gui
{
class GraphEditorWidget;

class MainWindow : public QMainWindow
{
	public:
	MainWindow();
	~MainWindow() override;

	private slots:
	void OpenGraph();

	private:
	void InitializeGUI();
	void InitializeMenus();

	pagoda::Pagoda m_pagoda;
	GraphEditorWidget *m_graphEditor;

};  // class MainWindow
}  // namespace pgeditor::gui
