#pragma once

#include "collection.h"
#include "render_state.h"

namespace pgeditor::renderer {
class Renderer;

class RenderPass {
public:
  RenderPass(const Collection &collection);

  void Render(Renderer *r);

  RenderState &GetRenderState() { return m_renderState; }

private:
  Collection m_collection;
  RenderState m_renderState;
};
} // namespace pgeditor::renderer
