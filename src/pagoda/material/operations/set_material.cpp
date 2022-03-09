#include "set_material.h"

#include <pagoda/objects/parameter_callback.h>
#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>

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

const std::string SetMaterial::inputObject("geo");
const std::string SetMaterial::inputMaterial("material");
const std::string SetMaterial::outputObject("out");

SetMaterial::SetMaterial(objects::ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
}

SetMaterial::~SetMaterial()
{
}

void SetMaterial::SetParameters(objects::ParameterCallback* cb)
{
}

void SetMaterial::Interfaces(objects::InterfaceCallback* cb)
{
  cb->InputInterface(m_inputGeometry, inputObject, "Geo", Interface::Arity::Many);
  cb->InputInterface(m_inputMaterial, inputMaterial, "Material", Interface::Arity::One);
  cb->OutputInterface(m_outputGeometry, outputObject, "Out", Interface::Arity::Many);
}

const std::string& SetMaterial::GetOperationName() const
{
	static const std::string sName{"SetMaterial"};
	return sName;
}

void SetMaterial::DoWork()
{
	START_PROFILE;

	auto materialSystem =
	  m_proceduralObjectSystem->GetComponentSystem<MaterialSystem>();
	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

  // Geometry
  auto inGeo = m_inputGeometry->GetNext();
  auto inGeometryComponent = geometrySystem->GetComponentAs<GeometryComponent>(inGeo);

  // Material
  auto inMat = m_inputMaterial->Get();
  auto inMaterialComponent = materialSystem->GetComponentAs<MaterialComponent>(inMat);

  auto outObject = CreateOutputProceduralObject(inGeo);
  auto newMaterialComponent = materialSystem->CreateComponentAs<MaterialComponent>(outObject);
  newMaterialComponent->SetMaterial(inMaterialComponent->GetMaterial());

  m_outputGeometry->SetNext(outObject);
}
}  // namespace pagoda::material::operations
