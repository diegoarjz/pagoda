#pragma once

#include <pagoda/math/vec_base.h>

#include <pgeditor/renderer/collection.h>

#include <memory>

namespace pagoda::scene {
class Camera;
}

namespace pgeditor::renderer::metal {
class MetalRenderer {
public:
  MetalRenderer(void *CAMetalLayerHandle);
  ~MetalRenderer();

  void InitRenderer();

  void Draw(const Collection &c);

  void SetCamera(pagoda::scene::Camera &cam);

  void SetDisplaySize(const pagoda::math::Vec2U &size);

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
} // namespace pgeditor::renderer::metal
