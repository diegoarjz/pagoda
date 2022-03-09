#include "material_component.h"

#include "material_system.h"

namespace pagoda::material
{
std::string MaterialComponent::GetComponentSystemName() { return MaterialSystem::GetComponentSystemName(); }

MaterialComponent::~MaterialComponent() {}

std::string MaterialComponent::GetType() const { return GetComponentSystemName(); }

void MaterialComponent::SetMaterial(const MaterialNetworkPtr& m) { m_material = m; }
const MaterialNetworkPtr& MaterialComponent::GetMaterial() const { return m_material; }
MaterialNetworkPtr& MaterialComponent::GetMaterial() { return m_material; }
}  // namespace pagoda::material
