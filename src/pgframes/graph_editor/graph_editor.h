#pragma once

#include "windows/window.h"

#include <memory>

namespace alpha::frontend {
class GraphEditor : public Window {
public:
  GraphEditor();
  ~GraphEditor() override;

  DockHint GetDockHint() const override { return DockHint::BottomPanel; }

protected:
  const std::string &WindowName() override;
  bool Draw() override;

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
} // namespace alpha::frontend

