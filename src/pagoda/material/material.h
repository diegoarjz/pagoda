#pragma once

#include <memory>
#include <unordered_map>

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

	void SetTexture(int slot, const std::string &textureFile);
	void SetTexture(int slot, const image::ImagePtr texture);

private:
	std::unordered_map<int, image::ImagePtr> m_textures;
};
}  // namespace material
}  // namespace pagoda
