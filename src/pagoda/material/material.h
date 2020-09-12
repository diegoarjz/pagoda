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

	void SetTexture(uint32_t slot, const std::string &textureFile);
	void SetTexture(uint32_t slot, const image::ImagePtr texture);
	image::ImagePtr GetTexture(uint32_t slot) const;
	void TexturesEach(const std::function<void(uint32_t, image::ImagePtr)> &fn) const;

	void SetShaderSource(ShaderType type, const std::string &source);
	void ShadersEach(const std::function<void(ShaderType, const std::string &)> &fn);
	std::size_t GetShaderCount() const;

	void SetAttribute(const std::string &name, dynamic::DynamicValueBasePtr value);
	void AttributesEach(const std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> &fn) const;

	private:
	std::unordered_map<uint32_t, image::ImagePtr> m_textures;
	std::unordered_map<ShaderType, std::string> m_shaderSources;
	std::unordered_map<std::string, dynamic::DynamicValueBasePtr> m_materialAttributes;
};
}  // namespace material
}  // namespace pagoda
