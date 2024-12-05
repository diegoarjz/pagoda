#include "renderable.h"

namespace pagoda::renderer
{
Renderable::Renderable(const gl::MeshPtr& mesh, const gl::ShaderProgramPtr& shader)
  : m_mesh{mesh}
  , m_shader{shader}
{
}

const gl::MeshPtr& Renderable::GetMesh() const { return m_mesh; }
const gl::ShaderProgramPtr& Renderable::GetShader() const { return m_shader; }
const math::Mat4x4F& Renderable::GetWorldMatrix() const { return m_worldMatrix; }
void Renderable::SetWorldMatrix(const math::Mat4x4F& matrix) { m_worldMatrix = matrix; }
}
