#include "pgframes/main_loop.h"

#include <imgui.h>
#include <imgui_internal.h>


namespace pgframes {
MainLoop::MainLoop(WindowManagerPtr windowManager) : m_windowManager{windowManager} {}

bool MainLoop::RunMainLoop() {
  m_windowManager->SetupLayout();
  m_windowManager->DrawOpenWindows();
  return !m_windowManager->QuitRequested();
}
} // namespace alpha::frontend

