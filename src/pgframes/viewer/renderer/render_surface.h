#pragma once

#include "pagoda/math/vec_base.h"
#include <memory>

namespace pagoda::renderer::gl {
using FrameBufferPtr = std::shared_ptr<class FrameBuffer>;
}

namespace pagoda::scene {
using RenderTargetPtr = std::shared_ptr<class RenderTarget>;
}

namespace pagoda::renderer
{
class RenderSurface
{
public:
  RenderSurface(float width, float height);
  RenderSurface(const math::Vec2F& size);

  void Present(const pagoda::scene::RenderTargetPtr& renderTarget);

  void Resize(float width, float height);

  gl::FrameBufferPtr GetFrameBuffer() const;

private:
  math::Vec2F m_size;
  gl::FrameBufferPtr m_frameBuffer;
};
using RenderSurfacePtr = std::shared_ptr<RenderSurface>;

}
