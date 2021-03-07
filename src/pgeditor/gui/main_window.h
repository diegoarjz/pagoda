#pragma once

#include <pagoda/pagoda.h>

#include <QMainWindow>
#include <QWidget>
#include <QtCore>

namespace pgeditor::gui
{
class GraphEditorWidget;

/**
 * Pagoda GUI Editor main window.
 */
class MainWindow : public QMainWindow
{
	public:
	MainWindow();
	~MainWindow() override;

	private slots:
	void OpenGraph();
	void SaveGraph();

	private:
	void InitializeGUI();
	void InitializeMenus();

	pagoda::Pagoda m_pagoda;
	GraphEditorWidget *m_graphEditor;

};  // class MainWindow
}  // namespace pgeditor::gui
