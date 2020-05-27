#include "shader.h"

#include "gl_debug.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/unimplemented.h>
#include <pagoda/common/instrument/profiler.h>

#include <GL/glew.h>

#include <vector>

namespace pgeditor::rendering
{
Shader::Shader::Shader(ShaderType type) : m_shaderType(type) {}
void Shader::SetShaderSource(const std::string& source) { m_shaderSource = source; }

Shader::~Shader() {}

const std::string& Shader::GetShaderSource() const { return m_shaderSource; }

Shader::ShaderType Shader::GetType() const { return m_shaderType; }

GLuint Shader::GetShaderId() const { return m_shaderId; }

GLenum convert_shader_type(Shader::ShaderType type)
{
	switch (type)
	{
		case Shader::ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case Shader::ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
	}
}

void Shader::DoLoad(Renderer* r)
{
	START_PROFILE;

	const char* source = GetShaderSource().c_str();

	CHECK_GL_ERROR(m_shaderId = glCreateShader(convert_shader_type(GetType())));

	LOG_DEBUG("Compiling Shader " << m_shaderId);
	CHECK_GL_ERROR(glShaderSource(m_shaderId, 1, &source, NULL));
	CHECK_GL_ERROR(glCompileShader(m_shaderId));

	GLint compileStatus;
	CHECK_GL_ERROR(glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &compileStatus));
	if (compileStatus == GL_FALSE)
	{
		GLint logLength;
		CHECK_GL_ERROR(glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &logLength));
		std::vector<char> compilationLog(logLength + 1);
		CHECK_GL_ERROR(glGetShaderInfoLog(m_shaderId, logLength, NULL, &compilationLog[0]));
		LOG_ERROR("Shader compilation failed:");
		LOG_ERROR(&compilationLog[0]);
	}
	LOG_DEBUG("Done compiling shader");
}

void Shader::DoRender(Renderer* r)
{
	//
}

void Shader::DoDispose(Renderer* r)
{
	//
	START_PROFILE;

	LOG_DEBUG("Disposing Shader " << m_shaderId);
	CHECK_GL_ERROR(glDeleteShader(m_shaderId));
}

}  // namespace pgeditor::rendering
