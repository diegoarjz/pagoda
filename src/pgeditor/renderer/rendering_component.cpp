#include "rendering_component.h"

#include "rendering_system.h"

namespace pgeditor::renderer {
std::string RenderingComponent::GetComponentSystemName() {
  return RenderingSystem::GetComponentSystemName();
}

void RenderingComponent::SetRenderable(const RenderablePtr &renderable) {
  m_renderable = renderable;
}
const RenderablePtr &RenderingComponent::GetRenderable() const {
  return m_renderable;
};
} // namespace pgeditor::renderer
