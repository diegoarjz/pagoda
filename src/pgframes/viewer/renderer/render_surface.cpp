#include "render_surface.h"

#include "gl/frame_buffer.h"

#include "pagoda/scene/render_target.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

namespace pagoda::renderer
{
RenderSurface::RenderSurface(float width, float height)
  : m_size{width, height}
  , m_frameBuffer{std::make_shared<gl::FrameBuffer>(m_size)}
{
}

RenderSurface::RenderSurface(const math::Vec2F& size)
  : m_size{size}
  , m_frameBuffer{std::make_shared<gl::FrameBuffer>(m_size)}
{
}

void RenderSurface::Present(const scene::RenderTargetPtr& renderTarget) {
  gl::FrameBufferPtr frameBuffer = nullptr;
  auto renderTargetHandle = renderTarget->GetHandle();
  if (renderTargetHandle.type() == typeid(gl::FrameBufferPtr)) {
    frameBuffer = std::any_cast<gl::FrameBufferPtr>(renderTargetHandle);
  }

  if (frameBuffer == nullptr) {
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Could not get FrameBuffer");
    return;
  }

  if (!m_frameBuffer->Blit(frameBuffer)) {
    return;
  }
  const ImTextureID textureId = (void*)(intptr_t)m_frameBuffer->getFrameTexture();
  ImGui::Image(textureId,
      ImVec2(X(m_size), Y(m_size)),
      ImVec2(0, 1),
      ImVec2(1, 0));
}

void RenderSurface::Resize(float width, float height) {
  if (m_size != math::Vec2F{width, height}) {
    X(m_size) = width;
    Y(m_size) = height;
    m_frameBuffer->RescaleFrameBuffer(width, height);
  }
}

gl::FrameBufferPtr RenderSurface::GetFrameBuffer() const { return m_frameBuffer; }
}
