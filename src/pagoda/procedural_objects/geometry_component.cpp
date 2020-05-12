#include "geometry_component.h"

namespace pagoda
{
using namespace geometry::core;

std::string GeometryComponent::GetComponentSystemName() { return GeometrySystem::GetComponentSystemName(); }

const Scope& GeometryComponent::GetScope() const { return m_scope; }

void GeometryComponent::SetScope(const Scope& scope) { m_scope = scope; }
}  // namespace pagoda
