#include "set_shader.h"

#include <pagoda/common/exception/exception.h>

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
using namespace common::exception;

const std::string SetShader::inputObject("in");
const std::string SetShader::outputObject("out");

SetShader::SetShader(objects::ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	CreateInputInterface(inputObject);
	CreateOutputInterface(outputObject);

	RegisterValues({{"type", std::make_shared<String>()}, {"path", std::make_shared<String>()}});
}

SetShader::~SetShader() {}

const std::string& SetShader::GetOperationName() const
{
	static const std::string sName{"SetShader"};
	return sName;
}

void SetShader::DoWork()
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

		UpdateValue("type");
		UpdateValue("path");

		std::string type = get_value_as<std::string>(*GetValue("type"));
		Material::ShaderType shaderType = Material::ShaderType::Vertex;
		if (type == "vertex") {
			shaderType = Material::ShaderType::Vertex;
		} else if (type == "tesselation") {
			shaderType = Material::ShaderType::Tesselation;
		} else if (type == "geometry") {
			shaderType = Material::ShaderType::Geometry;
		} else if (type == "fragment") {
			shaderType = Material::ShaderType::Fragment;
		} else {
			throw Exception("Unknown shader type '" + type + "'.");
		}

		materialComponent->GetMaterial().SetShaderSource(shaderType, get_value_as<std::string>(*GetValue("path")));
	}
}
}  // namespace pagoda::material::operations
