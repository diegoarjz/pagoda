#include "set_material.h"

#include <pagoda/material/material_component.h>
#include <pagoda/material/material_system.h>
#include <memory>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>
#include <pagoda/objects/hierarchical_component.h>
#include <pagoda/objects/hierarchical_system.h>

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/string_value.h>

namespace pagoda::material::operations
{
using namespace dynamic;
using namespace geometry;
using namespace objects;

const std::string SetMaterial::inputObject("in");
const std::string SetMaterial::outputObject("out");

SetMaterial::SetMaterial(objects::ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(inputObject);
	CreateOutputInterface(outputObject);

	// clang-format off
    RegisterValues({
        {"texture", std::make_shared<String>()}
    });
	// clang-format on
}

SetMaterial::~SetMaterial() {}

void SetMaterial::DoWork()
{
	START_PROFILE;

	auto materialSystem = m_proceduralObjectSystem->GetComponentSystem<MaterialSystem>();
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(inputObject))
	{
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputObject);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(outputObject);

		UpdateValue("texture");
		std::shared_ptr<MaterialComponent> materialComponent =
		    materialSystem->CreateComponentAs<MaterialComponent>(outObject);
		materialComponent->GetMaterial().SetTexture(get_value_as<std::string>(*GetValue("texture")));

		// geometry
		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
		auto geom = std::make_shared<Geometry>();
		*geom = *inGeometryComponent->GetGeometry();
		outGeometryComponent->SetGeometry(geom);
		outGeometryComponent->SetScope(inGeometryComponent->GetScope());

		// hierarchy
		auto parentHierarchicalComponent = hierarchicalSystem->GetComponentAs<HierarchicalComponent>(inObject);
		auto childHierarchicalComponent = hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(outObject);
		hierarchicalSystem->SetParent(parentHierarchicalComponent, childHierarchicalComponent);
	}
}
}  // namespace pagoda::material::operations
