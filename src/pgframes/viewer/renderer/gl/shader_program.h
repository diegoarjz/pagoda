#pragma once

#include "pagoda/math/matrix_base.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <memory>

namespace pagoda::renderer::gl
{
class ShaderProgram {
public:
  bool Load();
  bool Use();

  void SetModelMatrix(const math::Mat4x4F& mat);
  void SetViewMatrix(const math::Mat4x4F& mat);
  void SetProjectionMatrix(const math::Mat4x4F& mat);

private:
  GLuint m_shaderProgramId;

  // Uniforms
  GLuint m_modelMatrixUniform;
  GLuint m_viewMatrixUniform;
  GLuint m_projectionMatrixUniform;
};

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
}
