#include "pagoda/common/plugin.h"

#include "boost/filesystem.hpp"

#include "pagoda/common/fs/file_util.h"

namespace pagoda::common
{
  const std::string& Plugin::GetPluginPath()
  {
    using namespace boost::filesystem;
    static std::string pluginDir;
    if (pluginDir.empty()) {
      path executablePath = common::fs::GetExecutablePath();
      pluginDir = (executablePath.parent_path() / path("plugins")).string();
    }
    return pluginDir;
  }
}
