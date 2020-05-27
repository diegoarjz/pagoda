#pragma once

#include <memory>

namespace pagoda
{
namespace image
{
class Image;
using ImagePtr = std::shared_ptr<Image>;
}  // namespace image

namespace material
{
class Material
{
public:
	Material();

	void SetTexture(const std::string &textureFile);
	void SetTexture(const image::ImagePtr texture);

	image::ImagePtr GetTexture() const;

private:
	image::ImagePtr m_texture;
};
}  // namespace material
}  // namespace pagoda
