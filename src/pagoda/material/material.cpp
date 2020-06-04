#include "material.h"

#include <pagoda/image/image.h>
#include <cstdint>

#include <iostream>
#include <memory>
#include <vector>

using namespace pagoda::image;

namespace pagoda::material
{
Material::Material() {}

void Material::SetTexture(uint32_t slot, const std::string &textureFile)
{
	m_textures[slot] = std::make_shared<Image>(textureFile);
}

void Material::SetTexture(uint32_t slot, const image::ImagePtr texture) { m_textures[slot] = texture; }

image::ImagePtr Material::GetTexture(uint32_t slot) const
{
	auto tex = m_textures.find(slot);
	if (tex == m_textures.end())
	{
		return nullptr;
	}
	return tex->second;
}

void Material::TexturesEach(const std::function<void(uint32_t, image::ImagePtr)> &fn) const
{
	for (const auto &t : m_textures)
	{
		fn(t.first, t.second);
	}
}

void Material::SetShaderSource(ShaderType type, const std::string &source) { m_shaderSources[type] = source; }

void Material::ShadersEach(const std::function<void(ShaderType, const std::string &)> &fn)
{
	for (const auto &s : m_shaderSources)
	{
		fn(s.first, s.second);
	}
}

std::size_t Material::GetShaderCount() const { return m_shaderSources.size(); }

void Material::SetAttribute(const std::string &name, dynamic::DynamicValueBasePtr value)
{
	m_materialAttributes[name] = value;
}

void Material::AttributesEach(const std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> &fn) const
{
	for (const auto &m : m_materialAttributes)
	{
		fn(m.first, m.second);
	}
}
}  // namespace pagoda::material
