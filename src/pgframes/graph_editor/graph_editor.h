#pragma once

#include "pgframes/window.h"

#include <memory>

namespace pgframes::graph_editor {
class GraphEditor : public pgframes::Window {
public:
  GraphEditor();
  ~GraphEditor() override;

  pgframes::DockHint GetDockHint() const override { return pgframes::DockHint::BottomPanel; }

protected:
  const std::string &WindowName() override;
  bool Draw() override;

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
} // namespace pgframes::graph_editor
