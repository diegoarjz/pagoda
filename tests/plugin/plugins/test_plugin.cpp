#include "pagoda/common/plugin.h"
#include "pagoda//api.h"
#include "pagoda/pagoda.h"

#include <iostream>

class TestPlugin : public pagoda::common::Plugin {
public:
  TestPlugin() {}
  ~TestPlugin() override {}
  const char *Name() override { return "TestPlugin"; }

  bool Register(const pagoda::Pagoda* pagoda) override {
    return true;
  }
};

extern "C" PAGODA_PLUGIN pagoda::common::Plugin* CreatePlugin()
{
  return new TestPlugin;
}
