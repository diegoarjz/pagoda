#pragma once

#include "../../platform_window.h"

#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Metal/Metal.h>
#import <QuartzCore/QuartzCore.h>

namespace pgframes {
class MetalGlfwPlatform : public PlatformWindow {
public:
  bool Startup() override;
  void RunMainLoop() override;
  bool Teardown() override;

  PlatformTexturePtr CreateTexture(pagoda::image::ImagePtr image) override;

private:
  GLFWwindow *m_window{nullptr};
  float m_clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

  id<MTLDevice> m_device;
  id<MTLCommandQueue> m_commandQueue;
  CAMetalLayer *m_layer;
  MTLRenderPassDescriptor *m_renderPassDescriptor;

  friend class MetalTexture;
};
} // namespace alpha::frontend

