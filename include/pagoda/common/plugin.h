#pragma once

#include <filesystem>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

namespace pagoda {
class Pagoda;
}

namespace pagoda::common {
/**
 * A registry for plugins.
 *
 * A plugin json file has several fields that guide how a plugin is loaded.
 *
 *     {
 *      "pagodaVersion": "0.1",        Pagoda version the plugin was built for
 *      "name": "AlphaPagoda",         Plugin name
 *      "pluginFile": "alpha_pagoda",  Plugin binary minus the extension
 *      "registry": "Pagoda",          Class responsible for loading the plugin
 *      "method": "CreatePagodaPlugin" Function that creates the plugin
 *     }
 *
 * A plugin binary should define a function that returns a void pointer to an
 * object. It is the responsibility of the Plugin registry to know what to do
 * with that pointer.
 *
 *    extern "C" PAGODA_PLUGIN void* CreatePagodaPlugin() {
 *      return new alpha::pagoda::AlphaSystem;
 *    }
 */
class PluginRegistry {
public:
  class Directory {
  public:
    Directory(const std::filesystem::path &directory);

    const std::vector<std::filesystem::path> &GetPluginFiles() const;
    const std::filesystem::path &GetDirectory() const;
    bool IsLoaded() const;
    void Load();

  private:
    std::filesystem::path m_directory;
    bool m_loaded{false};
    std::vector<std::filesystem::path> m_pluginFiles;
  };

  class PluginInfo {
  public:
    PluginInfo(const std::filesystem::path &jsonFile);

    PluginInfo() = delete;
    PluginInfo(const PluginInfo&) = delete;
    PluginInfo& operator=(const PluginInfo&) = delete;

    const std::string &GetPluginName() const { return m_pluginName; }
    bool IsValid() const;

    template <class T> T *GetRegistrationClass() {
      if (m_method != nullptr) {
        return static_cast<T *>(m_method());
      }
      return nullptr;
    }

  private:
    friend class PluginRegistry;

    std::string m_pluginName;
    std::string m_registry;
    std::string m_methodName;
    std::filesystem::path m_pluginFile;
    void *(*m_method)(){nullptr};
  };
  using PluginInfoPtr = std::shared_ptr<PluginInfo>;

  static PluginRegistry &Instance();

  const std::list<Directory> &GetPluginLoadDirectories() const;
  bool HasPluginLoadDirectory(const std::filesystem::path &path);
  void AppendPluginLoadDirectory(const std::filesystem::path &path);
  void PrependPluginLoadDirectory(const std::filesystem::path &path);

  std::vector<PluginInfoPtr> GetPluginsForRegistry(const std::string &registry);

  void LoadAllPlugins();

  static std::string GetDefaultPluginExtension();

private:
  PluginRegistry();
  PluginRegistry(const PluginRegistry &) = delete;
  PluginRegistry &operator=(const PluginRegistry &) = delete;

  std::list<Directory> m_pluginLoadDirectories;
  std::unordered_map<std::string, PluginInfoPtr> m_plugins;
};

/**
 * Interface to register a plugin with pagoda.
 */
class PagodaPlugin {
public:
  virtual ~PagodaPlugin() {}

  /**
   * Called to perform the registration in the pagoda instance.
   */
  virtual bool Register(Pagoda *pagoda) = 0;
};
} // namespace pagoda::common
