#include "pagoda/common/plugin.h"

#include "boost/filesystem.hpp"

#include "pagoda/common/fs/file_util.h"

namespace pagoda::common {
const std::string &Plugin::GetPluginPath() {
  static const auto pluginDirEnv = std::getenv("PAGODA_PLUGIN_PATH");
  using namespace boost::filesystem;
  static std::string pluginDir;
  if (pluginDir.empty()) {
    if (pluginDirEnv != nullptr &&
        std::filesystem::is_directory(pluginDirEnv)) {
      pluginDir = pluginDirEnv;
    } else {
      path executablePath = common::fs::GetExecutablePath();
      pluginDir = (executablePath.parent_path() / path("plugins")).string();
    }
  }
  return pluginDir;
}
} // namespace pagoda::common
