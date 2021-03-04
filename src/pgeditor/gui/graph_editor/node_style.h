#pragma once

#include <QColor>

namespace pgeditor::gui
{
// clang-format off
  namespace node_style
  {
    namespace header
    {
      const char* const name_font_family = "Helvetica";
      const uint32_t name_font_size = 12;
      const char* const label_stylesheet = "background: transparent; color: white";
    }

    namespace node
    {
      const QColor border_color = QColor(0,0,0);
      const uint32_t border_width = 2;
      const uint32_t round_size = 5;

      const QColor fill_gradient_start = QColor(0,52,255);
      const QColor fill_gradient_end = QColor(0,40,192);
      const QColor selected_fill_gradient_start = QColor(255,0,0);
      const QColor selected_fill_gradient_end = QColor(156,0,0);

      const uint32_t ports_top_margin = 3;
      const uint32_t ports_bottom_margin = 3;
      const uint32_t ports_side_margin = 3;
      const uint32_t port_vertical_spacing = 3;
      const uint32_t port_horizontal_spacing = 3;
    }

    namespace port
    {
      const uint32_t horizontal_spacing = 3;
      const char* const name_font_family = "Helvetica";
      const uint32_t name_font_size = 10;
      const char* const label_stylesheet = "background: transparent; color: white";
    }

    namespace port_connection
    {
      const QColor border_color = QColor(0, 0, 0);
      const uint32_t border_width = 2;
      const QColor fill_color = QColor(255,255,255);
      const uint32_t port_size = 10;
    }

    namespace connector
    {
      const QColor line_color = QColor(0, 0, 0);
      const uint32_t line_width = 2;
      const uint32_t control_point_distance = 50;
    }
  }  // namespace node_style
// clang-format on
}  // namespace pgeditor::gui
