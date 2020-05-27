#include "material_system.h"

#include "material_component.h"

namespace pagoda::material
{
const std::string MaterialSystem::GetComponentSystemName() { return "MaterialSystem"; }

MaterialSystem::MaterialSystem() : ProceduralComponentSystem(GetComponentSystemName()) {}

MaterialSystem::~MaterialSystem() {}

void MaterialSystem::SetNamedMaterial(const std::string &name, const Material &material)
{
	m_namedMaterials.emplace(name, material);
}
const Material &MaterialSystem::GetNamedMaterial(const std::string &name) { return m_namedMaterials.at(name); }
}  // namespace pagoda::material
