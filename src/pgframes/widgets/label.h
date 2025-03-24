#pragma once

#include "pgframes/widgets/widget.h"

namespace pgframes::widgets {
class Label : public Widget {
public:
  Label(const std::string& label);
  ~Label();

  void SetText(const std::string& text);

  void Draw() override;

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
