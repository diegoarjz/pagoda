#pragma once

#include "pgframes/widgets/widget.h"
#include <functional>
#include <memory>
#include <vector>

namespace pgframes::widgets {
using MenuPtr = std::shared_ptr<class Menu>;

class Menu : public Widget {
public:
  using Callback_t = std::function<void(void)>;

  Menu();
  Menu(const std::string& label);
  Menu(const std::string& label, Callback_t cb);
  Menu(const std::string& label, const std::vector<MenuPtr>& subMenuItems);
  Menu(const std::string& label, Callback_t cb, const std::vector<MenuPtr>& subMenuItems);

  const std::string& GetLabel() const;

  void AddSubMenuItem(const MenuPtr& submenu);
  const std::vector<MenuPtr> GetSubMenuItems() const;

  void Draw() override;

private:
  const std::string m_label;
  Callback_t m_callback;
  std::vector<MenuPtr> m_subMenuItems;
};
}
