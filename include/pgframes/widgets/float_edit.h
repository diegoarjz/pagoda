#pragma once

#include "pgframes/widgets/widget.h"

namespace pgframes::widgets {
//----------------------------------------
// FloatEdit
class FloatEdit : public Widget {
public:
  FloatEdit(float value);
  ~FloatEdit();

  void Draw() override;

  using ValueChangedCalback_t = std::function<void(float)>;
  void OnValueChanged(ValueChangedCalback_t cb);
private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};

//----------------------------------------
// Float2Edit
class Float2Edit : public Widget {
public:
  Float2Edit(const pagoda::math::Vec2F& value);
  ~Float2Edit();

  void Draw() override;

  using ValueChangedCalback_t = std::function<void(const pagoda::math::Vec2F&)>;
  void OnValueChanged(ValueChangedCalback_t cb);
private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};

//----------------------------------------
// Float3Edit
class Float3Edit : public Widget {
public:
  Float3Edit(const pagoda::math::Vec3F& value);
  ~Float3Edit();

  void Draw() override;

  using ValueChangedCalback_t = std::function<void(const pagoda::math::Vec3F&)>;
  void OnValueChanged(ValueChangedCalback_t cb);
private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};

//----------------------------------------
// Float4Edit
class Float4Edit : public Widget {
public:
  Float4Edit(const pagoda::math::Vec4F& value);
  ~Float4Edit();

  void Draw() override;

  using ValueChangedCalback_t = std::function<void(const pagoda::math::Vec4F&)>;
  void OnValueChanged(ValueChangedCalback_t cb);
private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
