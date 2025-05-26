#include "pgframes/widgets/float_edit.h"

#include "pagoda/math/vec_base.h"

#include "pagoda/common/delegate.h"

#include "imgui.h"
#include "fmt/format.h"

namespace pgframes::widgets {

//----------------------------------------
// FloatEdit
class FloatEdit::Impl {
public:
  Impl()
  {}
  ~Impl() {}

  float m_float;

  pagoda::common::Delegate<void, float> m_onValueChanged;
};

FloatEdit::FloatEdit(float value)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_float = value;
}

FloatEdit::~FloatEdit() {}

void FloatEdit::Draw() {
  ImGui::SetNextItemWidth(X(CalculateSize()));

  bool changed = ImGui::DragFloat(fmt::format("##{}", m_id).c_str(), &m_impl->m_float);

  if (changed) {
    m_impl->m_onValueChanged(m_impl->m_float);
  }
}

void FloatEdit::OnValueChanged(ValueChangedCalback_t cb) {
  m_impl->m_onValueChanged.AddCallback(cb);
}

//----------------------------------------
// Float2Edit
class Float2Edit::Impl {
public:
  Impl()
  {}
  ~Impl() {}

  pagoda::math::Vec2F m_float;

  pagoda::common::Delegate<void, const pagoda::math::Vec2F&> m_onValueChanged;
};

Float2Edit::Float2Edit(const pagoda::math::Vec2F& value)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_float = value;
}

Float2Edit::~Float2Edit() {}

void Float2Edit::Draw() {
  ImGui::SetNextItemWidth(X(CalculateSize()));

  bool changed = ImGui::DragFloat2(fmt::format("##{}", m_id).c_str(), m_impl->m_float.a);

  if (changed) {
    m_impl->m_onValueChanged(m_impl->m_float);
  }
}

void Float2Edit::OnValueChanged(ValueChangedCalback_t cb) {
  m_impl->m_onValueChanged.AddCallback(cb);
}

//----------------------------------------
// Float3Edit
class Float3Edit::Impl {
public:
  Impl()
  {}
  ~Impl() {}

  pagoda::math::Vec3F m_float;

  pagoda::common::Delegate<void, const pagoda::math::Vec3F&> m_onValueChanged;
};

Float3Edit::Float3Edit(const pagoda::math::Vec3F& value)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_float = value;
}

Float3Edit::~Float3Edit() {}

void Float3Edit::Draw() {
  ImGui::SetNextItemWidth(X(CalculateSize()));

  bool changed = ImGui::DragFloat3(fmt::format("##{}", m_id).c_str(), m_impl->m_float.a);

  if (changed) {
    m_impl->m_onValueChanged(m_impl->m_float);
  }
}

void Float3Edit::OnValueChanged(ValueChangedCalback_t cb) {
  m_impl->m_onValueChanged.AddCallback(cb);
}

//----------------------------------------
// Float4Edit
class Float4Edit::Impl {
public:
  Impl()
  {}
  ~Impl() {}

  pagoda::math::Vec4F m_float;

  pagoda::common::Delegate<void, const pagoda::math::Vec4F&> m_onValueChanged;
};

Float4Edit::Float4Edit(const pagoda::math::Vec4F& value)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_float = value;
}

Float4Edit::~Float4Edit() {}

void Float4Edit::Draw() {
  ImGui::SetNextItemWidth(X(CalculateSize()));

  bool changed = ImGui::DragFloat4(fmt::format("##{}", m_id).c_str(), m_impl->m_float.a);

  if (changed) {
    m_impl->m_onValueChanged(m_impl->m_float);
  }
}

void Float4Edit::OnValueChanged(ValueChangedCalback_t cb) {
  m_impl->m_onValueChanged.AddCallback(cb);
}
}
