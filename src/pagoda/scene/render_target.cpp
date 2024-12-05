#include "render_target.h"

namespace pagoda::scene
{
  RenderTarget::RenderTarget(uint32_t width, uint32_t height, uint32_t depth)
    : m_width{width}
    , m_height{height}
    , m_depth{depth}
  {
  }
}
