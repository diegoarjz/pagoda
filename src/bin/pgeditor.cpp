#include <pgframes/platform_window.h>
#include <pgframes/main_loop.h>
#include <pgframes/window.h>

#include <pagoda/common/pluggable_factory.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

class PagodaEditor {
public:
  bool Init() {
    return true;
  }

  void ProcessArgs(int argc, char *argv[]) {}

  void Run() {
    auto& instance = pagoda::common::PluginRegistry::Instance();
    static const auto pluginDirEnv = std::getenv("PAGODA_PLUGIN_PATH");
    if (pluginDirEnv != nullptr) {
      instance.AppendPluginLoadDirectory(pluginDirEnv);
    }

    instance.LoadAllPlugins();
    m_platform = pgframes::PlatformWindow::Get("metal");
    if (m_platform == nullptr) {
      LOG_ERROR("Unable to create platform window");
      return;
    }

    m_windowManager = std::make_shared<pgframes::WindowManager>(m_platform);
    m_platform->SetMainLoop(std::make_shared<pgframes::MainLoop>(m_windowManager));

    m_platform->Startup();

    m_windowManager->CreateWindow("WindowBrowser");

    m_platform->RunMainLoop();

    m_platform->Teardown();
  }

private:
  std::shared_ptr<pgframes::PlatformWindow> m_platform;
  pgframes::WindowManagerPtr m_windowManager;
};

int main(int argc, char *argv[]) {
  PagodaEditor p;
  p.ProcessArgs(argc, argv);
  /*
  if (!p.Init()) {
    return 1;
  }
  */
  p.Run();
  return 0;
}
