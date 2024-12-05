#pragma once

#include "pagoda/math/vec_base.h"
#include "pagoda/scene/scene_node.h"

#include <memory>
#include <any>

namespace pagoda::scene
{
enum class PixelFormat {
  BGRA8Unorm
};

class RenderTarget : public SceneNode
{
public:
  using RenderTargetHandle = std::any;

  class AttachmentHandle;
  struct Attachment
  {
    AttachmentHandle* handle{nullptr};
    math::Vec4F clearColor;
    PixelFormat pixelFormat;
  };

  RenderTarget(uint32_t width, uint32_t height, uint32_t depth);

  void SetWidth(uint32_t width) { m_width = width; }
  uint32_t GetWidth() const { return m_width; }
  void SetHeight(uint32_t height) { m_height = height; }
  uint32_t GetHeight() const { return m_height; }
  void SetDepth(uint32_t depth) { m_depth = depth; }
  uint32_t GetDepth() const { return m_depth; }

  math::Vec3U GetDimensions() const { return math::Vec3U{m_width, m_height, m_depth}; }

  const Attachment& GetColorAttachment() const { return m_colorAttachment; }
  Attachment& GetColorAttachment() { return m_colorAttachment; }

  void SetHandle(RenderTargetHandle handle) { m_handle = handle; }
  RenderTargetHandle GetHandle() const { return m_handle; }

private:
  RenderTargetHandle m_handle{nullptr};
  Attachment m_colorAttachment;

  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_depth;
};

using RenderTargetPtr = std::shared_ptr<RenderTarget>;
}
