#pragma once

#include "bound_state.h"
#include "uniform.h"

#include <memory>
#include <vector>

namespace pgeditor::rendering
{
class Uniform;
using UniformPtr = std::shared_ptr<Uniform>;
class Shader;
using ShaderPtr = std::shared_ptr<Shader>;

class ShaderProgram
{
public:
	ShaderProgram();

	void SetVertexShader(ShaderPtr shader);
	ShaderPtr GetVertexShader() const;

	void SetFragmentShader(ShaderPtr shader);
	ShaderPtr GetFragmentShader() const;

	std::size_t GetBindableId() const;
	BoundState GetBoundState() const;
	void Bind(const std::size_t& id);
	void Unbind();

	UniformPtr CreateUniform(const std::string& name, const Uniform::Type type, const Uniform::Semantics semantics);
	const std::vector<UniformPtr>& GetUniforms() const;

private:
	ShaderPtr m_vertexShader;
	ShaderPtr m_fragmentShader;
	/// Bindable Id
	std::size_t m_bindableId;
	/// Bound state
	BoundState m_bound;
	std::vector<UniformPtr> m_uniforms;
};
}  // namespace pgeditor::rendering
