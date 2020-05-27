#include "material.h"

#include <pagoda/common/debug/logger.h>
#include <string>

#include "shader_program.h"
#include "texture.h"

namespace pgeditor::rendering
{
Material::Material(std::shared_ptr<ShaderProgram> program) : m_shaderProgram(program) {}

Material::~Material() {}

const std::vector<std::shared_ptr<Uniform>> &Material::GetUniforms() const { return m_shaderProgram->GetUniforms(); }

void Material::DoLoad(Renderer *r)
{
	m_shaderProgram->Load(r);

	for (const auto &t : m_textures)
	{
		t.second->Load(r);
	}
}

void Material::DoRender(Renderer *r)
{
	for (const auto &t : m_textures)
	{
		t.second->Render(r);
	}
	m_shaderProgram->Render(r);
}

void Material::DoDispose(Renderer *r) { m_shaderProgram->Dispose(r); }

void Material::SetTexture(uint8_t slot, std::shared_ptr<Texture> textue) { m_textures[slot] = textue; }
}  // namespace pgeditor::rendering
