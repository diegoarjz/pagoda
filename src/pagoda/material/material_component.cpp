#include "material_component.h"

#include "material_system.h"

namespace pagoda::material
{
std::string MaterialComponent::GetComponentSystemName() { return MaterialSystem::GetComponentSystemName(); }

MaterialComponent::~MaterialComponent() {}

std::string MaterialComponent::GetType() const { return GetComponentSystemName(); }

void MaterialComponent::SetMaterial(const Material& m) { m_material = m; }
const Material& MaterialComponent::GetMaterial() const { return m_material; }
Material& MaterialComponent::GetMaterial() { return m_material; }
}  // namespace pagoda::material
