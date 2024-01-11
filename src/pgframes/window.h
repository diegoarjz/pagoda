#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace pgframes {
class Window;
using WindowPtr = std::shared_ptr<Window>;
using WindowRef = std::weak_ptr<Window>;

class WindowManager;
using WindowManagerPtr = std::shared_ptr<WindowManager>;

class PlatformWindow;

enum class DockHint {
  Main,
  LeftPanel,
  RightPanel,
  BottomPanel
};

class Window {
public:
  virtual ~Window();

  bool DrawWindow();

  virtual void Close();

  virtual DockHint GetDockHint() const;

  WindowManagerPtr GetWindowManager() const;

protected:

  virtual bool Draw() = 0;
  virtual const std::string &WindowName() = 0;

  uint32_t m_identifier;
  bool m_initialized{false};

  WindowManagerPtr m_windowManager;
  friend class WindowManager;
};

class WindowManager : public std::enable_shared_from_this<WindowManager> {
public:
  WindowManager(std::shared_ptr<PlatformWindow> platform);

  //----------------------------------------
  //! @name Window Lifetime Management
  WindowRef CreateWindow(const std::string &type);
  void CloseWindow(WindowRef &w);
  std::vector<WindowPtr> GetOpenWindows();
  void DrawOpenWindows();

  //----------------------------------------
  //! @name Docking Management
  void SetupLayout();
  uint32_t IdForDockHint(DockHint hint);

  std::shared_ptr<PlatformWindow> GetPlatformWindow() const {return m_platform; };
private:
  std::unordered_map<DockHint, uint32_t> m_docks;
  std::shared_ptr<PlatformWindow> m_platform;
};

} // namespace pgframes

