#include "geometry_component.h"

#include "geometry_system.h"

namespace pagoda::geometry
{
using namespace geometry::core;

std::string GeometryComponent::GetComponentSystemName() { return GeometrySystem::GetComponentSystemName(); }

const Scope& GeometryComponent::GetScope() const { return m_scope; }

void GeometryComponent::SetScope(const Scope& scope) { m_scope = scope; }
}  // namespace pagoda::geometry
