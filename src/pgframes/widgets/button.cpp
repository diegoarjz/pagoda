#include "pgframes/widgets/button.h"

#include "pagoda/common/delegate.h"

#include "imgui.h"

namespace pgframes::widgets {
class Button::Impl {
public:
  Impl()
  {}
  ~Impl() {}

  void Pressed() {
    m_pressed();
  }

  std::string m_label;

  pagoda::common::Delegate<void> m_pressed;
};

Button::Button(const std::string& label)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_label = label;
}

Button::~Button() {}

void Button::Draw() {
  ImGui::SetNextItemWidth(X(CalculateSize()));

  if (ImGui::Button(m_impl->m_label.c_str())) {
    m_impl->Pressed();
  }
}

void Button::OnPressed(std::function<void(void)> pressed) {
  m_impl->m_pressed.AddCallback(pressed);
}
}
