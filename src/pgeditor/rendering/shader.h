#pragma once

#include "renderable.h"

#include <string>

#include <GL/glew.h>

namespace pgeditor::rendering
{
class Shader : public Renderable
{
public:
	enum class ShaderType
	{
		Vertex,
		Fragment
	};

	Shader(ShaderType type);
	virtual ~Shader();

	void SetShaderSource(const std::string& source);
	const std::string& GetShaderSource() const;
	ShaderType GetType() const;

	GLuint GetShaderId() const;

protected:
	void DoLoad(Renderer* r) override;
	void DoRender(Renderer* r) override;
	void DoDispose(Renderer* r) override;

private:
	std::string m_shaderSource;
	ShaderType m_shaderType;
	GLuint m_shaderId;
};

using ShaderPtr = std::shared_ptr<Shader>;
}  // namespace pgeditor::rendering
