#include "renderable_object.h"

#include "vertex.h"

using namespace pagoda::math;
using namespace pgeditor::scene;

namespace pgeditor::rendering
{
RenderableObject::RenderableObject(std::shared_ptr<Geometry> geometry, std::shared_ptr<ShaderProgram> shaderProgram)
    : m_geometry(geometry), m_shaderProgram(shaderProgram)
{
}

std::shared_ptr<Geometry> RenderableObject::GetGeometry() const { return m_geometry; }
std::shared_ptr<ShaderProgram> RenderableObject::GetShaderProgram() const { return m_shaderProgram; }

void RenderableObject::SetModelMatrix(const boost::qvm::mat<float, 4, 4>& modelMatrix) { m_modelMatrix = modelMatrix; }

const boost::qvm::mat<float, 4, 4>& RenderableObject::GetModelMatrix() const { return m_modelMatrix; }

void RenderableObject::SetViewMatrix(const boost::qvm::mat<float, 4, 4>& viewMatrix) { m_viewMatrix = viewMatrix; }
const boost::qvm::mat<float, 4, 4>& RenderableObject::GetViewMatrix() const { return m_viewMatrix; }

void RenderableObject::SetProjectionMatrix(const boost::qvm::mat<float, 4, 4>& projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;
}

const boost::qvm::mat<float, 4, 4>& RenderableObject::GetProjectionMatrix() const { return m_projectionMatrix; }
}  // namespace pgeditor::rendering
