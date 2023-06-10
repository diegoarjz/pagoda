#include "plugin.h"

#include "pagoda/common/plugin.h"
#include "pagoda/pagoda.h"

#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  auto &instance = pagoda::common::PluginRegistry::Instance();

  static const auto pluginDirEnv = std::getenv("PAGODA_PLUGIN_PATH");
  if (pluginDirEnv != nullptr) {
    instance.AppendPluginLoadDirectory(pluginDirEnv);
  }

  instance.LoadAllPlugins();

  const auto &plugins = instance.GetPluginsForRegistry("Pagoda");
  if (plugins.size() != 1) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Unable to load any plugin" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    return 1;
  }

  auto *mockPlugin =
      plugins.front()->GetRegistrationClass<tests::plugin::TestPlugin>();
  if (mockPlugin == nullptr ||
      mockPlugin->GetMessage() != "Created a Test Plugin") {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Unable to get plugin registration object" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    return 1;
  }
  return 0;
}
