#include "pagoda/pagoda.h"
#include "pagoda/common/plugin.h"

#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {

  std::cout << "Loading plugins from: " << pagoda::common::Plugin::GetPluginPath() << std::endl;

  uint32_t successfulAttempts = 0;
  for (int i = 0; i < 2; ++i) {
    pagoda::Pagoda p;

    p.CreateGraph();

    const auto &plugins = p.GetPluginInfo();
    bool found = false;
    for (const auto &plugin : plugins) {
      std::cout << "Plugin '" << plugin.name << "' @" << plugin.path
                << std::endl;
      found |= plugin.name == "TestPlugin";
    }
    found &= (plugins.size() == 1);

    successfulAttempts += found ? 1 : 0;
  }

  if (successfulAttempts != 2) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Unable to load any plugin" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
  }

  return successfulAttempts == 2 ? 0 : 1;
}
