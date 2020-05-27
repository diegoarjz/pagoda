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

void Material::SetTexture(const std::string &textureFile) { m_texture = std::make_shared<Image>(textureFile); }

void Material::SetTexture(const image::ImagePtr texture) { m_texture = texture; }

ImagePtr Material::GetTexture() const { return m_texture; }
}  // namespace pagoda::material
