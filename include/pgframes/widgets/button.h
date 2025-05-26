#pragma once

#include "pgframes/widgets/widget.h"

namespace pgframes::widgets {
class Button : public Widget {
public:
  Button(const std::string& label);
  ~Button();

  void Draw() override;

  void OnPressed(std::function<void()> pressed);

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
