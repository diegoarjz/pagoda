#include "pgframes/widgets/menu.h"
#include <imgui.h>

namespace pgframes::widgets {
Menu::Menu() {
}

Menu::Menu(const std::string& label)
  : m_label{label}
{
}

Menu::Menu(const std::string& label, Callback_t cb) 
  : m_label{label}
  , m_callback{cb}
{
}

Menu::Menu(const std::string& label, const std::vector<MenuPtr>& subMenuItems)
  : m_label{label}
  , m_subMenuItems{subMenuItems}
{
}

Menu::Menu(const std::string& label, Callback_t cb, const std::vector<MenuPtr>& subMenuItems)
  : m_label{label}
  , m_callback{cb}
  , m_subMenuItems{subMenuItems}
{
}

const std::string& Menu::GetLabel() const {
  return m_label;
}

void Menu::AddSubMenuItem(const MenuPtr& submenu) {
  m_subMenuItems.push_back(submenu);
}

const std::vector<MenuPtr> Menu::GetSubMenuItems() const {
  return m_subMenuItems;
}

void Menu::Draw() {
  const auto& menuItem = GetSubMenuItems();

  if (m_label.empty()) {
    if (ImGui::BeginMenuBar()) {
      for (const auto& m : menuItem) {
        m->Draw();
      }
      ImGui::EndMenuBar();
    }
  }
  else {
    if (menuItem.empty()) {
      if (ImGui::MenuItem(GetLabel().c_str())) {
        if (m_callback) {
          m_callback();
        }
      }
    }
    else {
      if (ImGui::BeginMenu(GetLabel().c_str())) {
        for (const auto& m : menuItem) {
          m->Draw();
        }
        ImGui::EndMenu();
      }
    }
  }
}
}
