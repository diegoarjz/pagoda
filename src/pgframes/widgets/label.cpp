#include "label.h"

#include "imgui.h"

namespace pgframes::widgets {
class Label::Impl {
public:
  Impl() {}
  ~Impl() {}

  std::string m_label;
};

Label::Label(const std::string& label)
  : m_impl{std::make_unique<Impl>()}
{
  m_impl->m_label = label;
}

Label::~Label() {}

void Label::SetText(const std::string& text) {
  m_impl->m_label = text;
}

void Label::Draw() {
  ImGui::Text("%s", m_impl->m_label.c_str());
}
}
