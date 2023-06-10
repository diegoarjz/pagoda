#pragma once

#include <string>

namespace tests::plugin {
  class TestPlugin {
    public:
      virtual std::string GetMessage() const = 0;
  };
}
