#include "pgframes/widgets/color_edit.h"

#include "pagoda/common/delegate.h"

#include "pagoda/math/vec_base.h"

#include "imgui.h"
#include "fmt/format.h"
#include <boost/qvm/gen/swizzle4.hpp>

namespace pgframes::widgets {
class ColorEdit::Impl {
public:
  Impl()
  {}
  ~Impl() {}

  pagoda::math::Vec4F m_color;
  bool m_isColor4;

  pagoda::common::Delegate<void, pagoda::math::Vec4F> m_onColorChanged;
};

ColorEdit::ColorEdit(const pagoda::math::Vec3F& color3)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_color = boost::qvm::XYZ1(color3);
  m_impl->m_isColor4 = false;
}

ColorEdit::ColorEdit(const pagoda::math::Vec4F& color4)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_color = color4;
  m_impl->m_isColor4 = true;
}

ColorEdit::~ColorEdit() {}

void ColorEdit::Draw() {
  bool valueChanged = false;
  ImGui::SetNextItemWidth(X(CalculateSize()));

  if (m_impl->m_isColor4) {
    valueChanged = ImGui::ColorEdit4(fmt::format("##{}", m_id).c_str(), m_impl->m_color.a);
  }
  else {
    valueChanged = ImGui::ColorEdit3(fmt::format("##{}", m_id).c_str(), m_impl->m_color.a);
  }

  if (valueChanged) {
    m_impl->m_onColorChanged(m_impl->m_color);
  }
}

void ColorEdit::OnColorChanged(std::function<void(const pagoda::math::Vec4F&)> cb) {
  m_impl->m_onColorChanged.AddCallback(cb);
}
}
