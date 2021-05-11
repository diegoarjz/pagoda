#include "default_material.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/unimplemented.h>
#include <pagoda/common/instrument/profiler.h>

#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>

#include <vector>

using namespace pagoda::math;

namespace pgeditor::renderer::gl
{
namespace
{
GLuint compileShader(GLenum shaderType, const char* source)
{
	GLuint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);
	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if (compileStatus == GL_FALSE) {
		GLint logLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> compilationLog(logLength + 1);
		glGetShaderInfoLog(shaderId, logLength, NULL, &compilationLog[0]);
		LOG_ERROR("Shader compilation failed:");
		LOG_ERROR(&compilationLog[0]);
		LOG_ERROR("Source");
		LOG_ERROR(source);
		LOG_ERROR("----------------------------------------");

		return 0;
	}

	return shaderId;
}

void setUniform(GLint location, const Mat4x4F& m)
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

const char* vsSource = R"(
#version 410

layout(location = 0) in vec3 vertexPosition_modelSpace;
layout(location = 1) in vec3 vertexNormal_modelSpace;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 vertexPos;
out vec4 vertexNormal;

void main() {
  vertexPos = viewMatrix * modelMatrix * vec4(vertexPosition_modelSpace, 1.0);
  gl_Position = projectionMatrix * vertexPos;
  vertexNormal = viewMatrix * modelMatrix * vec4(vertexNormal_modelSpace, 0);
}
)";

const char* fsSource = R"(
#version 410

in vec4 vertexPos;
in vec4 vertexNormal;

layout(location = 0) out vec4 outColor;

void main() {
  vec4 lightPos = vec4(0, 0, 0, 1);
  vec4 vecToLight = normalize(lightPos - vertexPos);
  float diffuseFactor = dot(vecToLight, normalize(vertexNormal));
  vec4 ambientLight = vec4(0.1, 0.1, 0.1, 1.0);

  outColor = ambientLight +
             diffuseFactor * vec4(0.92, 0.86, 0.7, 1.0);
}
)";
}  // namespace

void DefaultMaterial::Load()
{
	LOG_INFO("Creating shader program");
	GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

	LOG_INFO("Compiled shaders. Vertex: " << vs << " Fragment: " << fs);
	if (vs != 0 && fs != 0) {
		m_shaderProgramId = glCreateProgram();
		glAttachShader(m_shaderProgramId, vs);
		glAttachShader(m_shaderProgramId, fs);
		glLinkProgram(m_shaderProgramId);

		GLint linkStatus;
		glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE) {
			LOG_ERROR("Unable to link shader program " << m_shaderProgramId);
			GLint logLength;
			glGetProgramiv(m_shaderProgramId, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0) {
				std::vector<char> errorMessage(logLength + 1);
				glGetProgramInfoLog(m_shaderProgramId, logLength, NULL, &errorMessage[0]);
				LOG_ERROR(&errorMessage[0]);
			}
		}
	}

	glDetachShader(m_shaderProgramId, vs);
	glDetachShader(m_shaderProgramId, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	m_projectionMatrixUniform = glGetUniformLocation(m_shaderProgramId, "projectionMatrix");
	m_viewMatrixUniform = glGetUniformLocation(m_shaderProgramId, "viewMatrix");
	m_modelMatrixUniform = glGetUniformLocation(m_shaderProgramId, "modelMatrix");

	LOG_INFO("Shader uniforms:");
	LOG_INFO(" Projection Matrix: " << m_projectionMatrixUniform);
	LOG_INFO(" View Matrix: " << m_viewMatrixUniform);
	LOG_INFO(" Model Matrix: " << m_modelMatrixUniform);
}

void DefaultMaterial::Render()
{
	using namespace boost::qvm;

	START_PROFILE;
	glUseProgram(m_shaderProgramId);

	setUniform(m_modelMatrixUniform, identity_mat<float, 4>());
	setUniform(m_projectionMatrixUniform, m_projectionMatrix);
	setUniform(m_viewMatrixUniform, m_viewMatrix);
}

void DefaultMaterial::Dispose()
{
	glDeleteProgram(m_shaderProgramId);
	m_shaderProgramId = 0;
	m_modelMatrixUniform = 0;
	m_viewMatrixUniform = 0;
	m_projectionMatrixUniform = 0;
}

void DefaultMaterial::SetProjectionMatrix(const pagoda::math::Mat4x4F& mat) { m_projectionMatrix = mat; }
void DefaultMaterial::SetViewMatrix(const pagoda::math::Mat4x4F& mat) { m_viewMatrix = mat; }
void DefaultMaterial::SetModelMatrix(const pagoda::math::Mat4x4F& mat) { m_modelMatrix = mat; }
}  // namespace pgeditor::renderer::gl
