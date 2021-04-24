#include "rendering_component.h"

#include "rendering_system.h"

namespace pgeditor::renderer
{
std::string RenderingComponent::GetComponentSystemName() { return RenderingSystem::GetComponentSystemName(); }
}  // namespace pgeditor::renderer
