#include "pagoda/common/plugin.h"

#include "boost/filesystem.hpp"

#include "pagoda/common/debug/logger.h"
#include "pagoda/common/fs/file_util.h"

#include <nlohmann/json.hpp>

#include <boost/dll.hpp>

#include <iostream>

namespace pagoda::common {

static std::map<std::filesystem::path, boost::dll::shared_library>
    loadedLibraries;

PluginRegistry::PluginInfo::PluginInfo(const std::filesystem::path &jsonFile) {
  try {
    const auto jsonContents = nlohmann::json::parse(
        fs::LoadFileToString(boost::filesystem::path(jsonFile)));

    const auto jsonDir = jsonFile.parent_path();

    if (jsonContents.find("pagodaVersion") != jsonContents.end()) {
      m_pluginName = jsonContents["name"].get<std::string>();
      m_pluginFile = jsonDir / (jsonContents["pluginFile"].get<std::string>() +
                                PluginRegistry::GetDefaultPluginExtension());
      m_registry = jsonContents["registry"].get<std::string>();
      m_methodName = jsonContents["method"].get<std::string>();
    } else {
      return;
    }
  } catch (...) {
    LOG_ERROR("Unable to read Plugin Info " << jsonFile);
    return;
  }

  if (!std::filesystem::is_regular_file(m_pluginFile)) {
    LOG_ERROR("File " << m_pluginFile << " is not a plugin type");
    return;
  }

  try {
    boost::dll::shared_library library;
    auto iter = loadedLibraries.find(m_pluginFile);
    if (iter != loadedLibraries.end()) {
      library = iter->second;
    } else {
      library =
          boost::dll::shared_library(boost::filesystem::path(m_pluginFile),
                                     boost::dll::load_mode::default_mode);
      loadedLibraries.emplace(m_pluginFile, library);
    }
    if (!library.has(m_methodName)) {
      LOG_ERROR("Plugin " << m_pluginFile << " doesn't have the "
                          << m_methodName << " method.");
      return;
    }

    m_method = library.get<void *()>(m_methodName);
    if (m_method == nullptr) {
      return;
    }

  } catch (const boost::wrapexcept<boost::system::system_error> &e) {
    LOG_ERROR("Unable to load plugin " << m_pluginFile << " with error:");
    LOG_ERROR(e.what());
  }
}

bool PluginRegistry::PluginInfo::IsValid() const { return m_method != nullptr; }

PluginRegistry::Directory::Directory(const std::filesystem::path &directory)
    : m_directory{directory}, m_loaded{false} {}

const std::vector<std::filesystem::path> &
PluginRegistry::Directory::GetPluginFiles() const {
  return m_pluginFiles;
}
const std::filesystem::path &PluginRegistry::Directory::GetDirectory() const {
  return m_directory;
}

bool PluginRegistry::Directory::IsLoaded() const { return m_loaded; }

void PluginRegistry::Directory::Load() {
  std::filesystem::directory_iterator iter(m_directory);
  std::filesystem::directory_iterator end;

  while (iter != end) {
    const auto &filePath = *iter;
    if (std::filesystem::is_regular_file(filePath) &&
        iter->path().extension() == ".json") {
      // TODO: Do verification here
      m_pluginFiles.emplace_back(filePath);
    }
    ++iter;
  }

  m_loaded = true;
}

std::string PluginRegistry::GetDefaultPluginExtension() {
#if PAGODA_OS_MACOS
  static std::string sExtension{".dylib"};
#elif PAGODA_OS_WINDOWS
  static std::string sExtension{".dll"};
#elif PAGODA_OS_LINUX
  static std::string sExtension{".so"};
#endif
  return sExtension;
}
//

PluginRegistry::PluginRegistry() {}

PluginRegistry &PluginRegistry::Instance() {
  static PluginRegistry sInstance;
  return sInstance;
}

std::vector<PluginRegistry::PluginInfoPtr>
PluginRegistry::GetPluginsForRegistry(const std::string &registry) {
  std::vector<PluginRegistry::PluginInfoPtr> plugins;
  for (auto &p : m_plugins) {
    if (p.second->m_registry == registry) {
      plugins.push_back(p.second);
    }
  }
  return plugins;
}

const std::list<PluginRegistry::Directory> &
PluginRegistry::GetPluginLoadDirectories() const {
  return m_pluginLoadDirectories;
}

void PluginRegistry::AppendPluginLoadDirectory(
    const std::filesystem::path &path) {
  if (!std::filesystem::is_directory(path)) {
    LOG_ERROR("Path " << path.c_str() << " is not a directory.");
    return;
  }
  if (HasPluginLoadDirectory(path)) {
    LOG_WARNING("Already added " << path << " as a plugin directory.");
    return;
  }
  m_pluginLoadDirectories.push_back(path);
}

bool PluginRegistry::HasPluginLoadDirectory(const std::filesystem::path &path) {
  const auto iter = std::find_if(
      m_pluginLoadDirectories.begin(), m_pluginLoadDirectories.end(),
      [&path](const Directory &d) { return d.GetDirectory() == path; });
  return iter != m_pluginLoadDirectories.end();
}

void PluginRegistry::PrependPluginLoadDirectory(
    const std::filesystem::path &path) {
  if (!std::filesystem::is_directory(path)) {
    LOG_ERROR("Path " << path.c_str() << " is not a directory.");
    return;
  }
  if (HasPluginLoadDirectory(path)) {
    LOG_WARNING("Already added " << path << " as a plugin directory.");
    return;
  }
  m_pluginLoadDirectories.push_front(path);
}

void PluginRegistry::LoadAllPlugins() {
  for (auto &dir : m_pluginLoadDirectories) {
    if (!dir.IsLoaded()) {
      dir.Load();

      for (const auto &pluginFile : dir.GetPluginFiles()) {
        auto p = std::make_shared<PluginInfo>(pluginFile);
        if (p->IsValid()) {
          m_plugins.emplace(pluginFile, p);
        }
      }
    }
  }
}

} // namespace pagoda::common
