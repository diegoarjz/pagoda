#pragma once

#include "pgframes/viewer/renderer/renderer.h"

namespace pagoda::renderer
{
class MetalRenderer : public renderer::Renderer
{
  public:
    MetalRenderer();
    ~MetalRenderer() override;

    void InitRenderer() override;
    void StartFrame() override;
    void EndFrame() override;
    void StartRenderPass() override;
    void EndRenderPass() override;

    void SetUpRenderTarget(scene::RenderTargetPtr renderTarget) override;
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
}
