#pragma once

#include "gl/mesh.h"
#include "gl/shader_program.h"

namespace pagoda::renderer
{
class Renderable
{
public:
  Renderable(const gl::MeshPtr& mesh, const gl::ShaderProgramPtr& shader);

  const gl::MeshPtr& GetMesh() const;
  const gl::ShaderProgramPtr& GetShader() const;

  const math::Mat4x4F& GetWorldMatrix() const;
  void SetWorldMatrix(const math::Mat4x4F& matrix);

private:
  gl::MeshPtr m_mesh;
  gl::ShaderProgramPtr m_shader;
  math::Mat4x4F m_worldMatrix;
};
using RenderablePtr = std::shared_ptr<Renderable>;
}
