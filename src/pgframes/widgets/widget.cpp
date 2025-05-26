#include "pgframes/widgets/widget.h"

#include <boost/qvm/gen/swizzle4.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <entt/entt.hpp>
#include <iostream>

namespace pgframes::widgets {

Widget::Widget()
  : m_id{std::to_string(reinterpret_cast<uint64_t>(this))} {
}

void Widget::SetSizeHint(Size width, Size height)
{
  m_width = width;
  m_height = height;
}

pagoda::math::Vec2F Widget::CalculateSize() {
  // Calculate Size
  pagoda::math::Vec2F size{0, 0};
  const ImVec2 maxSize = ImGui::GetContentRegionMax();
  const ImVec2 availSize = ImGui::GetContentRegionAvail();

  if (m_width.type == Size::Type::Relative) {
    X(size) = m_width.size * maxSize.x;
  }
  else if (m_width.type == Size::Type::Absolute) {
    X(size) = m_width.size;
  }
  else {
    X(size) = availSize.x;
  }

  if (m_height.type == Size::Type::Relative) {
    Y(size) = m_height.size * maxSize.y;
  }
  else if (m_height.type == Size::Type::Absolute) {
    Y(size) = m_height.size;
  }
  else {
    Y(size) = availSize.y;
  }

  return size;
}

void GroupWidget::AddWdiget(const WidgetPtr& widget) {
  m_childWidgets.push_back(widget);
}

const std::list<WidgetPtr>& GroupWidget::GetWidgets() const { return m_childWidgets; }

void HorizontalLayout::Draw() {
  ImGui::BeginGroup();
  // TODO: Create a copy to avoid crashes when changing layouts
  auto childWidgetCopy = m_childWidgets;
  for (auto& w : childWidgetCopy) {
    w->Draw();
    if (w != m_childWidgets.back()) {
      ImGui::SameLine(0, 10);
    }
  }
  ImGui::EndGroup();
}

void VerticalLayout::Draw() {
  ImGui::BeginGroup();
  // TODO: Create a copy to avoid crashes when changing layouts
  auto childWidgetCopy = m_childWidgets;
  for (auto& w : childWidgetCopy) {
    w->Draw();
  }
  ImGui::EndGroup();
}
}
