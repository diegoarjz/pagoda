#include "extrude_geometry.h"

#include <pagoda/geometry/algorithms/extrusion.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>

#include <memory>

namespace pagoda::geometry::operations
{
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;

const char* ExtrudeGeometry::name = "ExtrudeGeometry";
const std::string ExtrudeGeometry::input_geometry("in");
const std::string ExtrudeGeometry::output_geometry("out");

ExtrudeGeometry::ExtrudeGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(input_geometry);
	CreateOutputInterface(output_geometry);

	RegisterValues({{"extrusion_amount", std::make_shared<FloatValue>(0.0f)}});
}

ExtrudeGeometry::~ExtrudeGeometry() {}

void ExtrudeGeometry::DoWork()
{
	START_PROFILE;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	while (HasInput(input_geometry))
	{
		ProceduralObjectPtr in_object = GetInputProceduralObject(input_geometry);
		UpdateValue("extrusion_amount");

		float extrusion_amount = get_value_as<float>(*GetValue("extrusion_amount"));
		Extrusion<Geometry> extrude(extrusion_amount);

		// Geometry
		ProceduralObjectPtr out_object = CreateOutputProceduralObject(in_object, output_geometry);
		std::shared_ptr<GeometryComponent> geometry_component =
		    geometrySystem->CreateComponentAs<GeometryComponent>(out_object);
		std::shared_ptr<GeometryComponent> in_geometry_component =
		    geometrySystem->GetComponentAs<GeometryComponent>(in_object);
		GeometryPtr in_geometry = in_geometry_component->GetGeometry();

		auto out_geometry = std::make_shared<Geometry>();
		extrude.Execute(in_geometry, out_geometry);

		geometry_component->SetGeometry(out_geometry);
		geometry_component->SetScope(
		    Scope::FromGeometryAndConstrainedRotation(out_geometry, in_geometry_component->GetScope().GetRotation()));
	}
}
}  // namespace pagoda::geometry::operations
