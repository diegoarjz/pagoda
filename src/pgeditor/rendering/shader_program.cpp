#include "shader_program.h"

#include "uniform.h"

namespace pgeditor::rendering
{
ShaderProgram::ShaderProgram() : m_bindableId(0), m_bound(BoundState::Unbound) {}

void ShaderProgram::SetVertexShader(ShaderPtr shader) { m_vertexShader = shader; }

ShaderPtr ShaderProgram::GetVertexShader() const { return m_vertexShader; }

void ShaderProgram::SetFragmentShader(ShaderPtr shader) { m_fragmentShader = shader; }

ShaderPtr ShaderProgram::GetFragmentShader() const { return m_fragmentShader; }

std::size_t ShaderProgram::GetBindableId() const { return m_bindableId; }
BoundState ShaderProgram::GetBoundState() const { return m_bound; }
void ShaderProgram::Bind(const std::size_t& id)
{
	m_bound = BoundState::Bound;
	m_bindableId = id;
}
void ShaderProgram::Unbind() { m_bound = BoundState::Unbound; }

UniformPtr ShaderProgram::CreateUniform(const std::string& name, const Uniform::Type type,
                                        const Uniform::Semantics semantics)
{
	auto uniform = std::make_shared<Uniform>(name, type, semantics);
	m_uniforms.push_back(uniform);
	return uniform;
}

const std::vector<UniformPtr>& ShaderProgram::GetUniforms() const { return m_uniforms; }
}  // namespace pgeditor::rendering
