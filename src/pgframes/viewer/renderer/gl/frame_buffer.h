#pragma once

#include "pagoda/math/vec_base.h"
#include <memory>

namespace pagoda::renderer::gl
{
using FrameBufferPtr = std::shared_ptr<class FrameBuffer>;
class FrameBuffer
{
public:
	FrameBuffer(const math::Vec2F& size);
	FrameBuffer(float width, float height);
	~FrameBuffer();
	unsigned int getFrameTexture() const;
	void RescaleFrameBuffer(float width, float height);
	void Bind() const;
	void Unbind() const;

  bool Blit(const FrameBufferPtr& framebufer);

  float GetWidth() const;
  float GetHeigh() const;
  math::Vec2F GetDimensions() const { return m_size; }

private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;
  math::Vec2F m_size;
};
}
