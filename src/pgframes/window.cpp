#include "window.h"

#include "pagoda/common/pluggable_factory.h"
#include <imgui.h>

namespace pgframes {
namespace {
class WindowFactory : public pagoda::common::PluggableFactory<Window, std::string> {
private:
  WindowFactory() : PluggableFactory("WindowFactory") {
  }

public:
  static std::unordered_map<uint32_t, WindowPtr> createdWindows;

  static WindowFactory &Instance() {
    static WindowFactory sInstance;
    return sInstance;
  }
};

std::unordered_map<uint32_t, WindowPtr> WindowFactory::createdWindows;

static uint32_t uniqueIdentifier{0};
} // namespace

Window::~Window() {}

bool Window::DrawWindow() {
  bool open = true;
  if (!m_initialized) {
    ImGui::SetNextWindowSize({600, 400});
    m_initialized = true;
  }

  ImGui::SetNextWindowDockID(m_windowManager->IdForDockHint(GetDockHint()));
  if (ImGui::Begin((WindowName() + std::to_string(m_identifier)).c_str(),
                   &open)) {
    if (!open) {
      Close();
      ImGui::End();
      return false;
    }

    Draw();

    ImGui::End();
  }
  return true;
}

void Window::Close() {
  WindowFactory::Instance().createdWindows.erase(m_identifier);
}

DockHint Window::GetDockHint() const { return DockHint::Main; }

WindowManagerPtr Window::GetWindowManager() const {
  assert(m_windowManager != nullptr);
  return m_windowManager;
}

WindowManager::WindowManager(std::shared_ptr<PlatformWindow> platform)
    : m_platform{platform} {}

WindowRef WindowManager::CreateWindow(const std::string &type) {
  if (auto w = WindowFactory::Instance().Create(type)) {
    w->m_identifier = uniqueIdentifier++;
    w->m_windowManager = shared_from_this();
    WindowFactory::createdWindows.emplace(w->m_identifier, w);
    return w;
  }
  return WindowRef{};
}

void WindowManager::CloseWindow(WindowRef &w) { w.lock()->Close(); }

std::vector<WindowPtr> WindowManager::GetOpenWindows() {
  std::vector<WindowPtr> windows;
  windows.reserve(WindowFactory::Instance().createdWindows.size());
  for (auto w : WindowFactory::Instance().createdWindows) {
    windows.push_back(w.second);
  }
  return windows;
}

void WindowManager::SetupLayout() {
  /*
  auto dockSpaceId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

  if (!m_layoutInitialized) {
    m_layoutInitialized = true;

    const auto viewportSize = ImGui::GetMainViewport()->Size;
    ImGui::DockBuilderRemoveNode(dockSpaceId);
    ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockSpaceId, viewportSize);

    auto leftPanel = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Left,
                                                 0.10f, NULL, &dockSpaceId);
    auto rightPanel = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right,
                                                  0.20f, NULL, &dockSpaceId);
    auto bottomPanel = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Down,
                                                   0.30f, NULL, &dockSpaceId);

    m_docks[DockHint::Main] = dockSpaceId;
    m_docks[DockHint::LeftPanel] = leftPanel;
    m_docks[DockHint::RightPanel] = rightPanel;
    m_docks[DockHint::BottomPanel] = bottomPanel;
  }
  */
}

void WindowManager::DrawOpenWindows() {
  for (auto &w : GetOpenWindows()) {
    w->DrawWindow();
  }
}

uint32_t WindowManager::IdForDockHint(DockHint hint) { return m_docks[hint]; }
} // namespace pgframes
