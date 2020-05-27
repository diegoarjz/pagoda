#pragma once

#include "renderable.h"

#include <memory>

#include <GL/glew.h>

namespace pagoda::image
{
class Image;
}

namespace pgeditor::rendering
{
class Texture : public Renderable
{
public:
	Texture();
	Texture(std::shared_ptr<pagoda::image::Image> image);
	virtual ~Texture();

protected:
	void DoLoad(Renderer *r) override;
	void DoRender(Renderer *r) override;
	void DoDispose(Renderer *r) override;

private:
	std::shared_ptr<pagoda::image::Image> m_image;
	GLuint m_textureId;
};
}  // namespace pgeditor::rendering
