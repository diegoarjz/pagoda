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

void Material::SetTexture(int slot, const std::string &textureFile)
{
	m_textures[slot] = std::make_shared<Image>(textureFile);
}

void Material::SetTexture(int slot, const image::ImagePtr texture) { m_textures[slot] = texture; }

image::ImagePtr Material::GetTexture(int slot)
{
	auto tex = m_textures.find(slot);
	if (tex == m_textures.end())
	{
		return nullptr;
	}
	return tex->second;
}

void Material::SetShaderSource(ShaderType type, const std::string &source) { m_shaderSources[type] = source; }

void Material::ShadersEach(std::function<void(ShaderType, const std::string &)> &fn)
{
	for (const auto &s : m_shaderSources)
	{
		fn(s.first, s.second);
	}
}

void Material::SetAttribute(const std::string &name, dynamic::DynamicValueBasePtr value)
{
	m_materialAttributes[name] = value;
}

void Material::AttributesEach(std::function<void(const std::string &, dynamic::DynamicValueBasePtr)> &fn)
{
	for (const auto &m : m_materialAttributes)
	{
		fn(m.first, m.second);
	}
}
}  // namespace pagoda::material
