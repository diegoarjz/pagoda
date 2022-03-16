#pragma once

#include <pagoda/math/vec_base.h>
#include <pagoda/scene/camera.h>

#include "collection.h"
#include "pgeditor/renderer/uniform_buffer.h"
#include "render_state.h"

namespace pgeditor::renderer {
class Renderer {
public:
  Renderer() {}
  virtual ~Renderer() {}

  virtual void InitRenderer() {}

  virtual void StartFrame() = 0;
  virtual void EndFrame() = 0;
  virtual void StartRenderPass() = 0;
  virtual void EndRenderPass() = 0;
  void SetRenderState(const RenderState &s);

  virtual void DrawImmediate(const Collection &c) {}
  virtual void Draw(const Collection &c) {}

  virtual void SetCamera(pagoda::scene::Camera &cam) {}
  virtual void SetDisplaySize(const pagoda::math::Vec2U &size) {}

protected:
  virtual void updateRenderState(const RenderState::Diff_t& changedState) = 0;

  RenderState m_lastRenderState;
  RenderState m_newRenderState;
};
} // namespace pgeditor::renderer
