#pragma once

#include <string>

namespace pgeditor::rendering
{
class Shader
{
public:
	enum class ShaderType
	{
		Vertex,
		Fragment
	};

	Shader(ShaderType type);

	void SetShaderSource(const std::string& source);
	const std::string& GetShaderSource() const;
	ShaderType GetType() const;

private:
	std::string m_shaderSource;
	ShaderType m_shaderType;
};

using ShaderPtr = std::shared_ptr<Shader>;
}  // namespace pgeditor::rendering
