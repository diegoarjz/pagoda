#include "pgframes/widgets/child_window.h"

#include "pgframes/widgets/menu.h"

#include <imgui.h>

namespace pgframes::widgets {
class ChildWindow::Impl {
public:

  WidgetPtr m_mainWidget;
  MenuPtr m_menu;
};

ChildWindow::ChildWindow()
  : m_impl{std::make_unique<Impl>()}
{
}

ChildWindow::~ChildWindow() {}

void ChildWindow::Draw() {
  const auto size = CalculateSize();
  ImGuiWindowFlags flags = 0;
  if (m_impl->m_menu != nullptr) {
    flags |= ImGuiWindowFlags_MenuBar;
  }
  if (ImGui::BeginChild(m_id.c_str(), {X(size), Y(size)}, true, flags)) {

    auto& menu = m_impl->m_menu;
    if (menu != nullptr) {
      menu->Draw();
    }

    if (m_impl->m_mainWidget != nullptr) {
      m_impl->m_mainWidget->Draw();
    }
  }
  ImGui::EndChild();
}

void ChildWindow::SetMainWidget(const WidgetPtr& widget) {
  m_impl->m_mainWidget = widget;
}

WidgetPtr ChildWindow::GetMainWidget() const {
  return m_impl->m_mainWidget;
}

void ChildWindow::SetMenu(const MenuPtr& menu) {
  m_impl->m_menu = menu;
}

}
