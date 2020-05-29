#pragma once

#include <pagoda/dynamic/dynamic_value_base.h>

#include <functional>
#include <memory>
#include <string>
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
	enum class ShaderType
	{
		Vertex,
		Tesselation,
		Geometry,
		Fragment
	};

	Material();

	void SetTexture(int slot, const std::string &textureFile);
	void SetTexture(int slot, const image::ImagePtr texture);
	image::ImagePtr GetTexture(int slot);

	void SetShaderSource(ShaderType type, const std::string &source);
	void ShadersEach(std::function<void(ShaderType, const std::string &)> &fn);

	void SetAttribute(const std::string &name, dynamic::DynamicValueBasePtr value);
	void AttributesEach(std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> &fn);

private:
	std::unordered_map<int, image::ImagePtr> m_textures;
	std::unordered_map<ShaderType, std::string> m_shaderSources;
	std::unordered_map<std::string, dynamic::DynamicValueBasePtr> m_materialAttributes;
};
}  // namespace material
}  // namespace pagoda
