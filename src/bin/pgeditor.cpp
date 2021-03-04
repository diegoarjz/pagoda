#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pgeditor/gui/main_window.h>

#include <QApplication>
#include <QWidget>
#include <QtCore>

using namespace boost;

using namespace pagoda;
using namespace pagoda::common;
using namespace pagoda::common::debug;
using namespace pagoda::common::instrument;

using namespace pgeditor::gui;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.showMaximized();
	/*
	QFile styleSheetFile(":/styles/style_sheet.qss");
	styleSheetFile.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(styleSheetFile.readAll());

	mainWindow.setStyleSheet(styleSheet);
	*/

	return app.exec();
}
