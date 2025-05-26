#pragma once

#include "pgframes/widgets/widget.h"
#include "pgframes/widgets/menu.h"

namespace pgframes::widgets {

class ChildWindow : public Widget {
public:
  ChildWindow();
  ~ChildWindow();

  void Draw() override;

  void SetMainWidget(const WidgetPtr& widget);
  WidgetPtr GetMainWidget() const;
  void SetMenu(const MenuPtr& menu);

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
