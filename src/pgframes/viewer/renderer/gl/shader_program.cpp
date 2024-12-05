#include "shader_program.h"

#include <pagoda/common/debug/logger.h>

#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>

#define GL_CHECK(cmd) \
cmd; \
{ \
	auto error = glGetError(); \
	if (error != GL_NO_ERROR) { \
    LOG_ERROR(#cmd) \
    LOG_FATAL(" error: " << error); \
	} \
}

namespace pagoda::renderer::gl {

const char *vertexShaderSource = R"(
#version 410
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

varying vec4 vertColor;

void main()
{
  vec4 vertexPos = viewMatrix * vec4(aPos, 1.0);
  gl_Position = projectionMatrix * vertexPos;
  vertColor = aColor;
}
)";
const char *fragmentShaderSource = R"(
#version 410
layout(location = 0) out vec4 outColor;
varying vec4 vertColor;
void main()
{
   outColor = vertColor;
}
)";

bool ShaderProgram::Load() {
  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GL_CHECK(glShaderSource(vertexShader, 1, &vertexShaderSource, NULL));
  GL_CHECK(glCompileShader(vertexShader));
  // check for shader compile errors
  int success;
  char infoLog[512];
  GL_CHECK(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
  if (!success) {
    GL_CHECK(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
    LOG_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog);
    return false;
  }
  // fragment shader
  GL_CHECK(unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));
  GL_CHECK(glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL));
  GL_CHECK(glCompileShader(fragmentShader));
  // check for shader compile errors
  GL_CHECK(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
  if (!success) {
    GL_CHECK(glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog));
    LOG_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog);
    return false;
  }
  // link shaders
  GL_CHECK(m_shaderProgramId = glCreateProgram());
  GL_CHECK(glAttachShader(m_shaderProgramId, vertexShader));
  GL_CHECK(glAttachShader(m_shaderProgramId, fragmentShader));
  GL_CHECK(glLinkProgram(m_shaderProgramId));
  // check for linking errors
  GL_CHECK(glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &success));
  if (!success) {
    GL_CHECK(glGetProgramInfoLog(m_shaderProgramId, 512, NULL, infoLog));
    LOG_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog);
    return false;
  }
  GL_CHECK(glDeleteShader(vertexShader));
  GL_CHECK(glDeleteShader(fragmentShader));

  // Get uniforms
  m_modelMatrixUniform = glGetUniformLocation(m_shaderProgramId, "modelMatrix");
  m_viewMatrixUniform = glGetUniformLocation(m_shaderProgramId, "viewMatrix");
  m_projectionMatrixUniform = glGetUniformLocation(m_shaderProgramId, "projectionMatrix");

  return true;
}

namespace {
void setUniform(GLint location, const math::Mat4x4F& m)
{
	// clang-format off
  GLfloat mat[16] = {
      A00(m), A10(m), A20(m), A30(m),
      A01(m), A11(m), A21(m), A31(m),
      A02(m), A12(m), A22(m), A32(m),
      A03(m), A13(m), A23(m), A33(m),
  };
	// clang-format on
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}
}
void ShaderProgram::SetModelMatrix(const math::Mat4x4F& mat) {
  setUniform(m_modelMatrixUniform, mat);
}

void ShaderProgram::SetViewMatrix(const math::Mat4x4F& mat) {
  setUniform(m_viewMatrixUniform, mat);
}

void ShaderProgram::SetProjectionMatrix(const math::Mat4x4F& mat) {
  setUniform(m_projectionMatrixUniform, mat);
}

bool ShaderProgram::Use() {
  GL_CHECK(glUseProgram(m_shaderProgramId));
  return false;
}
}
