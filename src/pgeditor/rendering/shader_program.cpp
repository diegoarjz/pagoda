#include "shader_program.h"

#include "gl_debug.h"
#include "shader.h"
#include "uniform.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/unimplemented.h>
#include <pagoda/common/instrument/profiler.h>
#include <memory>

#include <GL/glew.h>

namespace pgeditor::rendering
{
ShaderProgram::ShaderProgram() {}
ShaderProgram::~ShaderProgram() {}

void ShaderProgram::SetVertexShader(ShaderPtr shader) { m_vertexShader = shader; }

ShaderPtr ShaderProgram::GetVertexShader() const { return m_vertexShader; }

void ShaderProgram::SetFragmentShader(ShaderPtr shader) { m_fragmentShader = shader; }

ShaderPtr ShaderProgram::GetFragmentShader() const { return m_fragmentShader; }

const GLuint& ShaderProgram::GetShaderProgramId() const { return m_shaderProgramId; }

void ShaderProgram::DoLoad(Renderer* r)
{
	START_PROFILE;

	CHECK_GL_ERROR(m_shaderProgramId = glCreateProgram());
	LOG_DEBUG("Loading Shader Program " << m_shaderProgramId);

	GetVertexShader()->Load(r);
	GetFragmentShader()->Load(r);

	CHECK_GL_ERROR(glAttachShader(m_shaderProgramId, GetVertexShader()->GetShaderId()));
	CHECK_GL_ERROR(glAttachShader(m_shaderProgramId, GetFragmentShader()->GetShaderId()));

	LOG_DEBUG("Linking shader program " << m_shaderProgramId);
	CHECK_GL_ERROR(glLinkProgram(m_shaderProgramId));

	GLint linkStatus;
	CHECK_GL_ERROR(glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &linkStatus));
	if (linkStatus == GL_FALSE)
	{
		GLint logLength;
		CHECK_GL_ERROR(glGetProgramiv(m_shaderProgramId, GL_INFO_LOG_LENGTH, &logLength));
		if (logLength > 0)
		{
			std::vector<char> errorMessage(logLength + 1);
			CHECK_GL_ERROR(glGetProgramInfoLog(m_shaderProgramId, logLength, NULL, &errorMessage[0]));
			LOG_ERROR("Unable to link program");
			LOG_ERROR(&errorMessage[0]);
		}
	}

	for (auto& uniform : GetUniforms())
	{
		uniform->Load(r);
	}

	CHECK_GL_ERROR(glDetachShader(m_shaderProgramId, GetVertexShader()->GetShaderId()));
	CHECK_GL_ERROR(glDetachShader(m_shaderProgramId, GetFragmentShader()->GetShaderId()));

	GetVertexShader()->Dispose(r);
	GetFragmentShader()->Dispose(r);
}

void ShaderProgram::DoRender(Renderer* r)
{
	START_PROFILE;

	CHECK_GL_ERROR(glUseProgram(m_shaderProgramId));

	for (auto& uniform : m_uniforms)
	{
		uniform->Render(r);
	}
}

void ShaderProgram::DoDispose(Renderer* r)
{
	//
}

UniformPtr ShaderProgram::CreateUniform(const std::string& name, const Uniform::Type type,
                                        const Uniform::Semantics semantics)
{
	auto uniform =
	    std::make_shared<Uniform>(std::dynamic_pointer_cast<ShaderProgram>(shared_from_this()), name, type, semantics);
	m_uniforms.push_back(uniform);
	return uniform;
}

const std::vector<UniformPtr>& ShaderProgram::GetUniforms() const { return m_uniforms; }
}  // namespace pgeditor::rendering
