#include "set_texture.h"

#include <pagoda/material/material_component.h>
#include <pagoda/material/material_system.h>

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

const std::string SetTexture::inputObject("in");
const std::string SetTexture::outputObject("out");

SetTexture::SetTexture(objects::ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(inputObject);
	CreateOutputInterface(outputObject);
}

SetTexture::~SetTexture() {}

void SetTexture::SetParameters(graph::ExecutionArgumentCallback* cb) {}

const std::string& SetTexture::GetOperationName() const
{
	static const std::string sName{"SetTexture"};
	return sName;
}

void SetTexture::DoWork()
{
	START_PROFILE;

	auto materialSystem = m_proceduralObjectSystem->GetComponentSystem<MaterialSystem>();

	while (HasInput(inputObject)) {
		ProceduralObjectPtr inObject = GetInputProceduralObject(inputObject);
		ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject, outputObject);
		std::shared_ptr<MaterialComponent> materialComponent = materialSystem->GetComponentAs<MaterialComponent>(outObject);
		if (materialComponent == nullptr) {
			materialComponent = materialSystem->CreateComponentAs<MaterialComponent>(outObject);
		}

		UpdateValue("slot");
		UpdateValue("path");

		materialComponent->GetMaterial().SetTexture(get_value_as<int>(*GetValue("slot")),
		                                            get_value_as<std::string>(*GetValue("path")));
	}
}
}  // namespace pagoda::material::operations
