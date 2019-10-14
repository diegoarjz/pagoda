#include "extrude_geometry.h"

#include "procedural_object_system.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "parameter/parameter.h"
#include "procedural_component.h"

#include "../selector.h"

#include <memory>

namespace selector
{
const char* ExtrudeGeometry::name = "ExtrudeGeometry";
const InterfaceName ExtrudeGeometry::input_geometry = InterfaceName("in", 0);
const InterfaceName ExtrudeGeometry::output_geometry = InterfaceName("out", 0);

ExtrudeGeometry::ExtrudeGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateInputInterface(input_geometry);
	CreateOutputInterface(output_geometry);

	SetParameter("extrusion_amount", 0.0f);
}

ExtrudeGeometry::~ExtrudeGeometry() {}

void ExtrudeGeometry::Execute()
{
	START_PROFILE;
	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	while (HasInput(input_geometry))
	{
		ProceduralObjectPtr in_object = GetInputProceduralObject(input_geometry);

		float extrusion_amount = GetParameterAs<float>("extrusion_amount");
		Extrusion<Geometry> extrude(extrusion_amount);

		// Geometry
		ProceduralObjectPtr out_object = CreateOutputProceduralObject(output_geometry);
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

		// Hierarchy
		std::shared_ptr<HierarchicalComponent> in_hierarchical_component =
		    hierarchicalSystem->GetComponentAs<HierarchicalComponent>(in_object);
		std::shared_ptr<HierarchicalComponent> out_hierarchical_component =
		    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(out_object);

		hierarchicalSystem->SetParent(out_hierarchical_component, in_hierarchical_component);
	}
}
}  // namespace selector
