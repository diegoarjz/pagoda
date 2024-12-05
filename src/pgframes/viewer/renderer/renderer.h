#pragma once

#include "pagoda/math/vec_base.h"
#include <memory>

namespace pagoda::scene {
using RenderTargetPtr = std::shared_ptr<class RenderTarget>;
using SceneGraphPtr = std::shared_ptr<class SceneGraph>;
using CameraPtr = std::shared_ptr<class Camera>;
}

namespace pagoda::renderer
{
class Renderer
{
public:
  Renderer() = default;
  virtual ~Renderer() = default;

  virtual void InitRenderer() {}

  virtual void Render(const scene::SceneGraphPtr& scene, const scene::CameraPtr& camera) = 0;

  virtual void StartFrame() = 0;
  virtual void EndFrame() = 0;
  virtual void StartRenderPass() = 0;
  virtual void EndRenderPass() = 0;

  void SetClearColor(float r, float g, float b);
  void SetViewPort(int x, int y, int w, int h);

  virtual void SetUpRenderTarget(scene::RenderTargetPtr renderTarget) = 0;

protected:
  math::Vec3F m_clearColor;
  math::Vec4I m_viewPort;
};

using RendererPtr = std::shared_ptr<Renderer>;
}
