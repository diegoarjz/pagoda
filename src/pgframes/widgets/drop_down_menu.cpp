#include "pgframes/widgets/drop_down_menu.h"
#include "pagoda/common/delegate.h"
#include <imgui.h>

namespace pgframes::widgets {

class DropDownMenu::Impl {
public:
  Impl(const MenuItems& items)
    : m_menuItems{items}
  {
  }

  void UpdateSelection(std::size_t index) {
    if (m_selectedItem != index) {
      m_selectedItem = index;
      m_onSelectionChanged(m_menuItems[index]);
    }
  }

  MenuItems m_menuItems;
  std::size_t m_selectedItem{0};
  pagoda::common::Delegate<void, std::string> m_onSelectionChanged;
};


DropDownMenu::DropDownMenu(const MenuItems& items)
  : m_impl{std::make_unique<Impl>(items)}
{

}

DropDownMenu::~DropDownMenu()
{
}

void DropDownMenu::Draw() {
  ImGui::SetNextItemWidth(X(CalculateSize()));

  const auto selectedItemValue = m_impl->m_menuItems[m_impl->m_selectedItem];
  if (ImGui::BeginCombo(("##" + m_id).c_str(), selectedItemValue.c_str())) {
    for (std::size_t i = 0; i < m_impl->m_menuItems.size(); ++i) {
      const auto& item = m_impl->m_menuItems[i];
      if (ImGui::Selectable(item.c_str())) {
        m_impl->UpdateSelection(i);
      }
    }
    ImGui::EndCombo();
  }
}

void DropDownMenu::SetSelectedIndex(std::size_t index) {
  m_impl->UpdateSelection(index);
}

void DropDownMenu::OnSelectionChanged(SelectionChangedCallback_t cb)
{
  m_impl->m_onSelectionChanged.AddCallback(cb);
}
}
