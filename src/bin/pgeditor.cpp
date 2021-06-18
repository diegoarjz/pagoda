#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pgeditor/gui/main_window.h>

#include <boost/program_options.hpp>

#include <QApplication>
#include <QWidget>
#include <QtCore>

using namespace boost;
namespace po = boost::program_options;

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

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm);

int main(int argc, char* argv[])
{
	po::variables_map vm;
	if (!ParseCommandLine(argc, argv, &vm)) {
		return 0;
	}

	QApplication app(argc, argv);

	app.setStyleSheet(QString{sStyleSheet});

	MainWindow mainWindow;

	try {
		if (vm.count("file")) {
			mainWindow.LoadGraphFile(vm["file"].as<std::string>());
		}
	} catch (po::error& e) {
		LOG_ERROR(e.what());
	} catch (std::exception& e) {
		LOG_ERROR(e.what());
	}

	mainWindow.showMaximized();

	return app.exec();
}

bool ParseCommandLine(int argc, char* argv[], po::variables_map* out_vm)
{
	try {
		po::positional_options_description positionalOptions;
		positionalOptions.add("file", 1);

		po::options_description desc("Options");
		// clang-format off
        desc.add_options()
            ("help", "Print help message.")
            ("version", "Print version information and exit.")
            ("file", po::value<std::string>(), "Input Graph specification file.");
		// clang-format on

		po::store(po::command_line_parser(argc, argv).options(desc).positional(positionalOptions).run(), *out_vm);
		po::notify(*out_vm);

		return true;
	} catch (po::error& e) {
		LOG_FATAL(e.what());
		return false;
	} catch (std::exception& e) {
		LOG_FATAL(e.what());
		return false;
	}
}
