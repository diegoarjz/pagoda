#include "pgframes/widgets/checkbox.h"
#include "pagoda/common/delegate.h"
#include <imgui.h>

namespace pgframes::widgets {

class Checkbox::Impl {
public:
  Impl() {
  }
  ~Impl() {
  }

  bool m_checked{false};
  pagoda::common::Delegate<void, bool> m_changed;
};

Checkbox::Checkbox()
  : m_impl{std::make_unique<Impl>()}
{
}

Checkbox::~Checkbox() {}

void Checkbox::Draw() {
  if (ImGui::Checkbox(("##" + std::string(m_id)).c_str(), &m_impl->m_checked)) {
    m_impl->m_changed(m_impl->m_checked);
  }
}

void Checkbox::SetChecked(bool checked) {
  if (m_impl->m_checked != checked) {
    m_impl->m_checked = checked;
    m_impl->m_changed(checked);
  }
}
bool Checkbox::IsChecked() const {
  return m_impl->m_checked;
}

void Checkbox::OnChanged(std::function<void(bool)> cb) {
  m_impl->m_changed.AddCallback(cb);
}

}
