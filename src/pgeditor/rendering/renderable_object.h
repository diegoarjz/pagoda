#pragma once

#include <pagoda/math/matrix_base.h>

#include <memory>

namespace pgeditor::scene
{
class Geometry;
}

namespace pgeditor::rendering
{
class ShaderProgram;

class RenderableObject
{
public:
	RenderableObject(std::shared_ptr<scene::Geometry> geometry, std::shared_ptr<ShaderProgram> shaderProgram);

	std::shared_ptr<scene::Geometry> GetGeometry() const;
	std::shared_ptr<ShaderProgram> GetShaderProgram() const;

	void SetModelMatrix(const boost::qvm::mat<float, 4, 4>& modelMatrix);
	const boost::qvm::mat<float, 4, 4>& GetModelMatrix() const;

	void SetViewMatrix(const boost::qvm::mat<float, 4, 4>& viewMatrix);
	const boost::qvm::mat<float, 4, 4>& GetViewMatrix() const;

	void SetProjectionMatrix(const boost::qvm::mat<float, 4, 4>& projectionMatrix);
	const boost::qvm::mat<float, 4, 4>& GetProjectionMatrix() const;

private:
	std::shared_ptr<scene::Geometry> m_geometry;
	std::shared_ptr<ShaderProgram> m_shaderProgram;

	boost::qvm::mat<float, 4, 4> m_modelMatrix;
	boost::qvm::mat<float, 4, 4> m_viewMatrix;
	boost::qvm::mat<float, 4, 4> m_projectionMatrix;
};
}  // namespace pgeditor::rendering
