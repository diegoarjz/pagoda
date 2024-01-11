#include "main_loop.h"

#include "window.h"

#include <imgui.h>
#include <imgui_internal.h>


namespace pgframes {
MainLoop::MainLoop(WindowManagerPtr windowManager) : m_windowManager{windowManager} {}

void MainLoop::RunMainLoop() {
  /*
  m_windowManager->SetupLayout();
  m_windowManager->DrawOpenWindows();
  */
}
} // namespace alpha::frontend

