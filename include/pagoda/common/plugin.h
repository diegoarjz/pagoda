#pragma once

#include <string>

namespace pagoda
{
  class Pagoda;
}

namespace pagoda::common
{
  /**
   * Interface to register a plugin with pagoda.
   *
   * A plugin should define a function that creates an object of this class such as:
   *     
   *     extern "C" PAGODA_API pagoda::common::Plugin* CreatePlugin()
   *     {
   *       return new Plugin;
   *     }
   */
  class Plugin
  {
    public:
      virtual ~Plugin() {}

      /**
       * Returns the plugin name.
       */
      virtual const char* Name() = 0;

      /**
       * Called to perform the registration in the pagoda instance.
       */
      virtual bool Register(Pagoda* pagoda) = 0;

      /**
       * Returns the path to the directory where plugins are installed.
       */
      static const std::string& GetPluginPath();
  };
}
