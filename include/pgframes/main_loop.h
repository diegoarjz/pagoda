#pragma once

#include "pgframes/window.h"

#include <memory>

namespace pgframes {
class MainLoop {
public:
  MainLoop(WindowManagerPtr windowManager);
  virtual ~MainLoop() {}

  virtual bool RunMainLoop();
private:
  WindowManagerPtr m_windowManager;
};

using MainLoopPtr = std::shared_ptr<MainLoop>;
} // namespace alpha::frontend

