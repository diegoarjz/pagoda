#pragma once

#include "pgframes/viewer/renderer/renderer.h"

namespace pagoda::renderer
{
class GLRenderer : public renderer::Renderer
{
  public:

  GLRenderer();
  ~GLRenderer() override;

  void InitRenderer() override;

  void Render(const scene::SceneGraphPtr& scene, const scene::CameraPtr& camera) override;

  void StartFrame() override;
  void EndFrame() override;
  void StartRenderPass() override;
  void EndRenderPass() override;

  void SetUpRenderTarget(scene::RenderTargetPtr renderTarget) override;

  private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
}
