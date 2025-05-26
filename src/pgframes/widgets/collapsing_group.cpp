#include "pgframes/widgets/collapsing_group.h"

#include "imgui.h"
#include <fmt/format.h>

namespace pgframes::widgets {
  class CollapsingGroup::Impl {
    public:
      Impl(const std::string& header)
        : m_header{header}
      {
      }
      ~Impl()
      {
      }

      WidgetPtr m_widget;
      std::string m_header;
  };


  CollapsingGroup::CollapsingGroup(const std::string& label)
    : m_impl{std::make_unique<Impl>(label)}
  {
  }

  CollapsingGroup::~CollapsingGroup()
  {
  }

  void CollapsingGroup::Draw() {
    if (ImGui::CollapsingHeader(fmt::format("{}###{}", m_impl->m_header.c_str(), m_id).c_str())) {
      if (m_impl->m_widget != nullptr) {
        m_impl->m_widget->Draw();
      }
    }
  }

  void CollapsingGroup::SetWidget(const WidgetPtr& widget)
  {
    m_impl->m_widget = widget;
  }

  void CollapsingGroup::SetLabel(const std::string& label) {
    m_impl->m_header = label;
  }
}
