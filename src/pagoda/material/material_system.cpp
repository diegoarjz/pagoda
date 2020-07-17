#include "material_system.h"

#include "material_component.h"

#include "operations/set_material.h"
#include "operations/set_material_attribute.h"
#include "operations/set_shader.h"
#include "operations/set_texture.h"

#include <pagoda/pagoda.h>
#include <memory>

namespace pagoda::material
{
using namespace operations;

const std::string MaterialSystem::GetComponentSystemName() { return "MaterialSystem"; }

MaterialSystem::MaterialSystem() : ProceduralComponentSystem(GetComponentSystemName()) {}

MaterialSystem::~MaterialSystem() {}

void MaterialSystem::SetNamedMaterial(const std::string &name, const Material &material)
{
	m_namedMaterials.emplace(name, material);
}
const Material &MaterialSystem::GetNamedMaterial(const std::string &name) { return m_namedMaterials.at(name); }

void MaterialSystem::DoClone(std::shared_ptr<MaterialComponent> from, std::shared_ptr<MaterialComponent> to)
{
	to->GetMaterial() = from->GetMaterial();
}

void MaterialSystem::Registration(Pagoda *pagoda)
{
	LOG_TRACE(Core, "Registering " << GetComponentSystemName());
	auto operationFactory = pagoda->GetOperationFactory();
	auto objectSystem = pagoda->GetProceduralObjectSystem();

	objectSystem->RegisterProceduralComponentSystem(std::make_shared<MaterialSystem>());

	operationFactory->Register("SetMaterial", [objectSystem]() { return std::make_shared<SetMaterial>(objectSystem); });
	operationFactory->Register("SetTexture", [objectSystem]() { return std::make_shared<SetTexture>(objectSystem); });
	operationFactory->Register("SetShader", [objectSystem]() { return std::make_shared<SetShader>(objectSystem); });
	operationFactory->Register("SetMaterialAttribute",
	                           [objectSystem]() { return std::make_shared<SetMaterialAttribute>(objectSystem); });
}

}  // namespace pagoda::material
