#pragma once

#include "pgframes/platform_window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace pgframes {
class OpenGL3GlfwPlatform : public PlatformWindow {
public:
  bool Startup() override;
  void RunMainLoop() override;
  bool Teardown() override;

  PlatformTexturePtr CreateTexture(pagoda::image::ImagePtr image) override;

private:
  GLFWwindow *m_window{nullptr};
  float m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};
};
} // namespace alpha::frontend


