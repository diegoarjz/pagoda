#include "rendering_component.h"

#include "rendering_system.h"

namespace pgeditor::renderer
{
std::string RenderingComponent::GetComponentSystemName() { return RenderingSystem::GetComponentSystemName(); }

void RenderingComponent::SetMesh(pgeditor::renderer::gl::MeshPtr mesh) { m_mesh = mesh; }
const pgeditor::renderer::gl::MeshPtr& RenderingComponent::GetMesh() const { return m_mesh; }
}  // namespace pgeditor::renderer
