#include "texture.h"

#include "gl_debug.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/image/image.h>

namespace pgeditor::rendering
{
Texture::Texture() {}
Texture::Texture(std::shared_ptr<pagoda::image::Image> image) : m_image(image) {}

Texture::~Texture() {}

void Texture::DoLoad(Renderer *r)
{
	if (!m_image->IsLoaded())
	{
		m_image->Load();
	}

	CHECK_GL_ERROR(glGenTextures(1, &m_textureId));
	LOG_DEBUG("Loading texture with id " << m_textureId);
	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, m_textureId));
	std::vector<uint8_t> data;
	m_image->CopyImageData(data);
	auto dimensions = m_image->GetDimensions();
	LOG_DEBUG("Image dimensions: " << std::get<0>(dimensions) << "x" << std::get<1>(dimensions));
	CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, std::get<0>(dimensions), std::get<1>(dimensions), 0, GL_RGB,
	                            GL_UNSIGNED_BYTE, &(data[0])));
	CHECK_GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
}

void Texture::DoRender(Renderer *r)
{
	//
	CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, m_textureId));
}

void Texture::DoDispose(Renderer *r)
{
	//
}

}  // namespace pgeditor::rendering
