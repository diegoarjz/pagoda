#pragma once

#include "pgframes/widgets/widget.h"

namespace pgframes::widgets {
class Checkbox : public Widget {
public:

  Checkbox();
  ~Checkbox();

  void Draw() override;

  void SetChecked(bool checked);
  bool IsChecked() const;

  void OnChanged(std::function<void(bool)> cb);

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
