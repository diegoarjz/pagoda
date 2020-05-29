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
ImagePtr Material::GetTexture(int slot) const
{
	auto i = m_textures.find(slot);
	if (i == m_textures.end())
	{
		return nullptr;
	}
	return i->second;
}
}  // namespace pagoda::material
