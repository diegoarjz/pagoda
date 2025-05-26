#pragma once

#include "pgframes/widgets/widget.h"

namespace pgframes::widgets {
class DropDownMenu : public Widget {
public:
  using MenuItems = std::vector<std::string>;

  DropDownMenu(const MenuItems& items);
  ~DropDownMenu();

  void Draw() override;

  void SetSelectedIndex(std::size_t index);

  using SelectionChangedCallback_t = std::function<void(const std::string&)>;
  void OnSelectionChanged(SelectionChangedCallback_t cb);
private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
