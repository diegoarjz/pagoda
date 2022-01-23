#pragma once

#include <pagoda/math/vec_base.h>

#include <pgeditor/renderer/collection.h>
#include <pgeditor/renderer/renderer.h>

#include <memory>

namespace pagoda::scene {
class Camera;
}

namespace pgeditor::renderer::metal {
class MetalRenderer : public Renderer {
public:
  MetalRenderer(void *CAMetalLayerHandle);
  ~MetalRenderer() override;

  void InitRenderer() override;

  void StartFrame() override;
  void EndFrame() override;
  void StartRenderPass() override;
  void EndRenderPass() override;

  void Draw(const Collection &c) override;

  void SetCamera(pagoda::scene::Camera &cam) override;

  void SetDisplaySize(const pagoda::math::Vec2U &size) override;

protected:
  void updateRenderState(const RenderState::Diff_t& changedState) override;

private:
  class Impl;
  std::unique_ptr<Impl> m_impl;
};
} // namespace pgeditor::renderer::metal
