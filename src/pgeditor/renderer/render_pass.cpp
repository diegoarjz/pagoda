#include "render_pass.h"

#include "renderer.h"

namespace pgeditor::renderer {
RenderPass::RenderPass(const Collection &collection)
    : m_collection{collection} {}

void RenderPass::Render(Renderer *r) {
  r->SetRenderState(m_renderState);
  r->StartRenderPass();
  r->Draw(m_collection);
  r->EndRenderPass();
}
} // namespace pgeditor::renderer
