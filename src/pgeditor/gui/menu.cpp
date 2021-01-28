#include "menu.h"

namespace pgeditor::gui
{
Menu::MenuItem::MenuItem(const std::string& label) : m_label{label} {}

Menu::MenuItem::MenuItem(const std::string& label, const std::function<void()> action)
  : m_label{label}, m_action{action}
{
}

Menu::Menu(const std::string& label) : m_label{label} {}
Menu::Menu(const std::string& label, const std::vector<MenuItem>&& menuItems) : m_label{label}, m_menuItems{menuItems}
{
}

const std::string& Menu::GetLabel() const { return m_label; }

void Menu::AddMenuItem(MenuItem&& item) { m_menuItems.push_back(item); }
const std::vector<Menu::MenuItem>& Menu::GetMenuItems() const { return m_menuItems; }
}  // namespace pgeditor::gui
