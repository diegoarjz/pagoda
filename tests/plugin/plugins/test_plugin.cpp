#include "../plugin.h"

#include "pagoda/common/plugin.h"
#include "pagoda//api.h"
#include "pagoda/pagoda.h"

#include <iostream>

class TestPluginImpl : public tests::plugin::TestPlugin {
  public:
    std::string GetMessage() const override { return "Created a Test Plugin"; }
};

extern "C" PAGODA_PLUGIN void* CreatePlugin()
{
  return new TestPluginImpl;
}
