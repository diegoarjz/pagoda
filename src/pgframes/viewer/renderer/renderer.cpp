#include "renderer.h"

namespace pagoda::renderer
{
  void Renderer::SetClearColor(float r, float g, float b)
  {
    m_clearColor = math::Vec3F{r, g, b};
  }

  void Renderer::SetViewPort(int x, int y, int w, int h)
  {
    m_viewPort = math::Vec4I{x, y, w, h};
  }
}
