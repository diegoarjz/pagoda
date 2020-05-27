#pragma once

#include "renderable.h"
#include "uniform.h"

#include <GL/glew.h>

#include <memory>
#include <vector>

namespace pgeditor::rendering
{
class Uniform;
using UniformPtr = std::shared_ptr<Uniform>;
class Shader;
using ShaderPtr = std::shared_ptr<Shader>;

class ShaderProgram : public Renderable
{
public:
	ShaderProgram();
	virtual ~ShaderProgram();

	void SetVertexShader(ShaderPtr shader);
	ShaderPtr GetVertexShader() const;

	void SetFragmentShader(ShaderPtr shader);
	ShaderPtr GetFragmentShader() const;

	UniformPtr CreateUniform(const std::string& name, const Uniform::Type type, const Uniform::Semantics semantics);
	const std::vector<UniformPtr>& GetUniforms() const;

	const GLuint& GetShaderProgramId() const;

protected:
	void DoLoad(Renderer* r) override;
	void DoRender(Renderer* r) override;
	void DoDispose(Renderer* r) override;

private:
	ShaderPtr m_vertexShader;
	ShaderPtr m_fragmentShader;
	std::vector<UniformPtr> m_uniforms;
	GLuint m_shaderProgramId;
};
}  // namespace pgeditor::rendering
