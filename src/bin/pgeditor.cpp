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

static const char* sStyleSheet = R"(
  QMainWindow {
    background-color: #282828;
    color: #ebdbb2;
  }

  QMainWindow::separator {
    background: black;
    width: 2px;
    height: 2px;
}

  QWidget {
    background-color: #282828;
    color: #ebdbb2;
  }

  QToolBar {
    background: #282828;
    border: none;
  }
)";

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	app.setStyleSheet(QString{sStyleSheet});

	MainWindow mainWindow;
	mainWindow.showMaximized();

	return app.exec();
}
