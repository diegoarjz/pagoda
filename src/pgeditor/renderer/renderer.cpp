#include "renderer.h"

namespace pgeditor::renderer {

void Renderer::SetRenderState(const RenderState &s) {
  m_newRenderState = s;
  updateRenderState(m_newRenderState.ComputeDiff(m_lastRenderState));
  m_lastRenderState = s;
}

} // namespace pgeditor::renderer
