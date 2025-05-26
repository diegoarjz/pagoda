#pragma once

#include "pgframes/widgets/widget.h"

namespace pgframes::widgets {
class CollapsingGroup : public Widget {
public:
  CollapsingGroup(const std::string& label);
  ~CollapsingGroup();

  void Draw() override;

  void SetWidget(const WidgetPtr& widget);
  void SetLabel(const std::string& label);

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}

