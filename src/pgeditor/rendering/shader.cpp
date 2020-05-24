#include "shader.h"

namespace pgeditor::rendering
{
Shader::Shader::Shader(ShaderType type) : m_shaderType(type) {}
void Shader::SetShaderSource(const std::string& source) { m_shaderSource = source; }

const std::string& Shader::GetShaderSource() const { return m_shaderSource; }

Shader::ShaderType Shader::GetType() const { return m_shaderType; }
}  // namespace pgeditor::rendering
