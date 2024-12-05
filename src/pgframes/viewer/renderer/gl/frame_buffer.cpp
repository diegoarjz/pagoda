#include "frame_buffer.h"

#include "pagoda/common/debug/logger.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GL_CHECK(cmd) \
cmd; \
{ \
	auto error = glGetError(); \
	if (error != GL_NO_ERROR) { \
    LOG_ERROR(#cmd) \
    LOG_FATAL(" error: " << error); \
	} \
}

namespace pagoda::renderer::gl
{
FrameBuffer::FrameBuffer(const math::Vec2F& size)
  : FrameBuffer(X(size), Y(size))
{
}

FrameBuffer::FrameBuffer(float width, float height)
  : m_size{width, height}
{
	GL_CHECK(glGenFramebuffers(1, &fbo));
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

	GL_CHECK(glGenTextures(1, &texture));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));

	GL_CHECK(glGenRenderbuffers(1, &rbo));
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
	GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  }

	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

FrameBuffer::~FrameBuffer()
{
	GL_CHECK(glDeleteFramebuffers(1, &fbo));
	GL_CHECK(glDeleteTextures(1, &texture));
	GL_CHECK(glDeleteRenderbuffers(1, &rbo));
}

unsigned int FrameBuffer::getFrameTexture() const
{
	return texture;
}

void FrameBuffer::RescaleFrameBuffer(float width, float height)
{
  m_size = math::Vec2F{width, height};

  Bind();
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0));

	GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
	GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
	GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
  Unbind();
}

void FrameBuffer::Bind() const
{
	GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool FrameBuffer::Blit(const FrameBufferPtr& framebufer) {
  glBlitNamedFramebuffer(
      framebufer->fbo,
      fbo,
      0, 0,
      framebufer->GetWidth(), framebufer->GetHeigh(),
      0, 0,
      GetWidth(), GetHeigh(),
      GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT,
      GL_NEAREST);
  const auto error = glGetError();
	if (error != GL_NO_ERROR) {
    LOG_FATAL(" blitting error: " << error);
	}
  return error == GL_NO_ERROR;
}

float FrameBuffer::GetWidth() const { return X(m_size); }
float FrameBuffer::GetHeigh() const { return Y(m_size); }

}
