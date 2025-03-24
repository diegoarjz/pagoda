#pragma once

#include "pgframes/widgets/widget.h"

namespace pgframes::widgets {
class ColorEdit : public Widget {
public:
  ColorEdit(const pagoda::math::Vec3F& color3);
  ColorEdit(const pagoda::math::Vec4F& color4);
  ~ColorEdit();

  void Draw() override;

  using ColorChangedCallback_t = std::function<void(const pagoda::math::Vec4F&)>;
  void OnColorChanged(ColorChangedCallback_t cb);
private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
