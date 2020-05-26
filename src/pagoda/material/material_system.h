#pragma once

#include "material.h"

#include <pagoda/objects/procedural_component_system.h>

#include <string>
#include <unordered_map>

namespace pagoda
{
namespace material
{
class MaterialComponent;

class MaterialSystem : public objects::ProceduralComponentSystem<MaterialComponent>
{
public:
	static const std::string GetComponentSystemName();

	MaterialSystem();
	virtual ~MaterialSystem();

	void SetNamedMaterial(const std::string &name, const Material &material);
	const Material &GetNamedMaterial(const std::string &name);

private:
	std::unordered_map<std::string, Material> m_namedMaterials;
};
}  // namespace material
}  // namespace pagoda
