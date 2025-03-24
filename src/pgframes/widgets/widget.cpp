#include "widget.h"

#include <boost/qvm/gen/swizzle4.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <entt/entt.hpp>
#include <iostream>

namespace pgframes::widgets {

Widget::Widget()
  : m_id{std::to_string(reinterpret_cast<uint64_t>(this))} {
}

void GroupWidget::AddWdiget(const WidgetPtr& widget) {
  m_childWidgets.push_back(widget);
}

void HorizontalLayout::Draw() {
  ImGui::BeginGroup();
  for (auto& w : m_childWidgets) {
    w->Draw();
    if (w != m_childWidgets.back()) {
      ImGui::SameLine(0, 10);
    }
  }
  ImGui::EndGroup();
}

void VerticalLayout::Draw() {
  ImGui::BeginGroup();
  for (auto& w : m_childWidgets) {
    w->Draw();
  }
  ImGui::EndGroup();
}



}
