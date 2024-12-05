#pragma once

#include "pgframes/window.h"

#include <memory>

namespace pgframes::viewer {
class ViewerWindow : public Window {
public:
  ViewerWindow();
  ~ViewerWindow() override;

  DockHint GetDockHint() const override { return DockHint::Main; }

protected:
  const std::string &WindowName() override;
  bool Draw() override;

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
} // namespace pgframes
