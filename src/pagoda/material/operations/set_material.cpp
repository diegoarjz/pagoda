#include "set_material.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/material/material_component.h>
#include <pagoda/material/material_system.h>

#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/string_value.h>

#include <memory>
#include <string>
#include <unordered_map>

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
}

SetMaterial::~SetMaterial() {}

void SetMaterial::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	for (uint32_t i = 0u; i < 8; ++i) {
		auto texture = "texture_" + std::to_string(i);
		RegisterMember(texture, cb->StringArgument(texture.c_str(), ("Texture " + std::to_string(i)).c_str(), ""));
	}
}

const std::string& SetMaterial::GetOperationName() const
{
	static const std::string sName{"SetMaterial"};
	return sName;
}

void SetMaterial::Interfaces(InterfaceCallback* cb)
{
}

void SetMaterial::DoWork()
{
	START_PROFILE;

	auto materialSystem = m_proceduralObjectSystem->GetComponentSystem<MaterialSystem>();
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(inputObject)) {
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputObject);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject, outputObject);

		for (uint32_t i = 0u; i < 8; ++i) {
			std::string texture = "texture_" + std::to_string(i);
			UpdateValue(texture);
			std::shared_ptr<MaterialComponent> materialComponent =
			  materialSystem->CreateComponentAs<MaterialComponent>(outObject);
			auto value = GetValue(texture);
			if (value != nullptr) {
				materialComponent->GetMaterial().SetTexture(i, get_value_as<std::string>(*value));
			}
		}

		// geometry
		auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inObject);
		auto outGeometryComponent = geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
		auto geom = std::make_shared<core::Geometry>();
		*geom = *inGeometryComponent->GetGeometry();
		outGeometryComponent->SetGeometry(geom);
		outGeometryComponent->SetScope(inGeometryComponent->GetScope());
	}
}
}  // namespace pagoda::material::operations
