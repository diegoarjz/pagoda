#pragma once

#include "material.h"

#include <pagoda/objects/procedural_component_system.h>

#include <string>
#include <unordered_map>

namespace pagoda
{
class Pagoda;

namespace objects
{
class ProceduralObjectSystem;
using ProceduralObjectSystemPtr = std::shared_ptr<ProceduralObjectSystem>;
}  // namespace objects

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

	/**
	 * Performs the registration of the Material System with \p pagoda.
	 */
	static void Registration(Pagoda *pagoda);

	protected:
	void DoClone(std::shared_ptr<MaterialComponent> from, std::shared_ptr<MaterialComponent> to) override;

	private:
	std::unordered_map<std::string, Material> m_namedMaterials;
};
}  // namespace material
}  // namespace pagoda
