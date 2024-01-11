#pragma once

#include "window.h"

#include <memory>

namespace pgframes {
class MainLoop {
public:
  MainLoop(WindowManagerPtr windowManager);
  virtual ~MainLoop() {}

  virtual void RunMainLoop();
private:
  WindowManagerPtr m_windowManager;
};

using MainLoopPtr = std::shared_ptr<MainLoop>;
} // namespace alpha::frontend

