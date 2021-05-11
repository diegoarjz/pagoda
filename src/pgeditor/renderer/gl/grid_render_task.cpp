#include "grid_render_task.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/unimplemented.h>

#include <pagoda/math/vec_base.h>
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

layout(location = 0) in vec3 vertexPosition_screenSpace;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 vertexPos;
out vec3 nearPoint;
out vec3 farPoint;

vec3 unprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
  mat4 viewInv = inverse(view);
  mat4 projInv = inverse(projection);
  vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
  return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
  vertexPos = vertexPosition_screenSpace;
  nearPoint = unprojectPoint(
    vertexPos.x,
    vertexPos.y,
    0.0,
    viewMatrix,
    projectionMatrix).xyz; // unprojecting on the near plane

  farPoint = unprojectPoint(
    vertexPos.x,
    vertexPos.y,
    1.0,
    viewMatrix,
    projectionMatrix).xyz; // unprojecting on the far plane

  gl_Position = vec4(vertexPos, 1.0);
}
)";

const char* fsSource = R"(
#version 410

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 vertexPos;
in vec3 nearPoint;
in vec3 farPoint;

layout(location = 0) out vec4 outColor;

vec4 grid(vec3 fragPos3D, float scale) {
    vec2 coord = fragPos3D.xz * scale; // use the scale variable to set the distance between the lines
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
        color.x = 1.0;
    return color;
}

float computeDepth(vec3 pos) {
  vec4 clip_space_pos = projectionMatrix * viewMatrix * vec4(pos.xyz, 1.0);
  float clip_space_depth = clip_space_pos.z / clip_space_pos.w;

  float far = gl_DepthRange.far;
  float near = gl_DepthRange.near;

  float depth = (((far-near) * clip_space_depth) + near + far) / 2.0;

  return depth;
}

float computeLinearDepth(vec3 pos) {
  float far = gl_DepthRange.far;
  float near = gl_DepthRange.near;
  vec4 clip_space_pos = projectionMatrix * viewMatrix * vec4(pos.xyz, 1.0);
  float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
  float linearDepth = (2.0 * near * far) / (far + near - clip_space_depth * (far - near)); // get linear value between 0.01 and 100
  return linearDepth / far; // normalize
}

void main() {
  float t = -nearPoint.y / (farPoint.y - nearPoint.y);
  vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
  gl_FragDepth = computeDepth(fragPos3D);

  vec3 R = nearPoint + t * (farPoint-nearPoint);
  float spotlight = min(1.0, 1.5 - 0.02*length(R.xz));

  outColor = grid(fragPos3D, 1) * float(t > 0);
  outColor.a *= spotlight;
}
)";
}  // namespace

GridRenderTask::GridRenderTask()
  : m_gridVAO{0}, m_gridVBO{0}, m_gridShaderId{0}, m_projectionMatrixUniform{0}, m_viewMatrixUniform{0}
{
}

void GridRenderTask::Load()
{
	static const Vec3F gridPos[] = {{-1, -1, 0}, {1, -1, 0}, {1, 1, 0}, {-1, 1, 0}};
	static const GLuint gridIndices[] = {0, 1, 2, 0, 2, 3};

	// Create VAO
	glGenVertexArrays(1, &m_gridVAO);
	glBindVertexArray(m_gridVAO);

	// Create VBO
	glGenBuffers(1, &m_gridVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vec3F), gridPos, GL_STATIC_DRAW);

	// Position Attrib Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Create Element Buffer
	glGenBuffers(1, &m_gridElements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gridElements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), gridIndices, GL_STATIC_DRAW);

	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG_ERROR("Unable to create grid. GL Error: " << error);
	}

	glDisableVertexAttribArray(0);

	// Create the shader
	GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

	if (vs != 0 && fs != 0) {
		m_gridShaderId = glCreateProgram();
		glAttachShader(m_gridShaderId, vs);
		glAttachShader(m_gridShaderId, fs);
		glLinkProgram(m_gridShaderId);

		GLint linkStatus;
		glGetProgramiv(m_gridShaderId, GL_LINK_STATUS, &linkStatus);
		if (linkStatus == GL_FALSE) {
			LOG_ERROR("Unable to link shader program " << m_gridShaderId);
			GLint logLength;
			glGetProgramiv(m_gridShaderId, GL_INFO_LOG_LENGTH, &logLength);
			if (logLength > 0) {
				std::vector<char> errorMessage(logLength + 1);
				glGetProgramInfoLog(m_gridShaderId, logLength, NULL, &errorMessage[0]);
				LOG_ERROR(&errorMessage[0]);
			}
		}
	}

	glDetachShader(m_gridShaderId, vs);
	glDetachShader(m_gridShaderId, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	m_projectionMatrixUniform = glGetUniformLocation(m_gridShaderId, "projectionMatrix");
	m_viewMatrixUniform = glGetUniformLocation(m_gridShaderId, "viewMatrix");
}

void GridRenderTask::Run()
{
	START_PROFILE;

	glUseProgram(m_gridShaderId);

	setUniform(m_projectionMatrixUniform, m_projectionMatrix);
	setUniform(m_viewMatrixUniform, m_viewMatrix);

	// glBindBuffer(GL_ARRAY_BUFFER, m_gridVBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gridElements);
	glBindVertexArray(m_gridVAO);

	glEnableVertexAttribArray(0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG_ERROR("Unable to draw grid. GL Error: " << error);
	}

	glDisableVertexAttribArray(0);
}

void GridRenderTask::Dispose() { UNIMPLEMENTED; }

void GridRenderTask::SetProjectionMatrix(const pagoda::math::Mat4x4F& mat) { m_projectionMatrix = mat; }
void GridRenderTask::SetViewMatrix(const pagoda::math::Mat4x4F& mat) { m_viewMatrix = mat; }

}  // namespace pgeditor::renderer::gl
