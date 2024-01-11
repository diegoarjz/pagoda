#include "platform_window.h"

#include "pagoda/common/pluggable_factory.h"


namespace pgframes {

class PlatformWindowFactory
    : public pagoda::common::PluggableFactory<PlatformWindow> {
public:
  PlatformWindowFactory()
      : pagoda::common::PluggableFactory<PlatformWindow>("PlatformWindow") {
    //
  }
  ~PlatformWindowFactory() override = default;
};

PlatformWindowPtr PlatformWindow::Get(const std::string &name) {
  static PlatformWindowPtr platformWindow = nullptr;
  if (platformWindow == nullptr) {
    PlatformWindowFactory factory;
    platformWindow = factory.Create(name);
  }
  return platformWindow;
}

} // namespace alpha::frontend
