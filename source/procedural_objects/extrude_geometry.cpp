#include "extrude_geometry.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "parameter/parameter.h"
#include "procedural_component.h"

#include <memory>

namespace selector
{
const char* ExtrudeGeometry::name = "ExtrudeGeometry";
const InterfaceName ExtrudeGeometry::input_geometry = InterfaceName("in", 0);
const InterfaceName ExtrudeGeometry::output_geometry = InterfaceName("out", 0);

ExtrudeGeometry::ExtrudeGeometry()
{
	START_PROFILE;

	ProceduralObjectMask in_geometry_mask;
	in_geometry_mask.set(static_cast<uint32_t>(GeometryComponent::GetType()));
	in_geometry_mask.set(static_cast<uint32_t>(HierarchicalComponent::GetType()));
	CreateInputInterface(input_geometry, in_geometry_mask);

	ProceduralObjectMask out_geometry_mask;
	out_geometry_mask.set(static_cast<uint32_t>(GeometryComponent::GetType()));
	out_geometry_mask.set(static_cast<uint32_t>(HierarchicalComponent::GetType()));
	CreateOutputInterface(output_geometry, out_geometry_mask);
}

ExtrudeGeometry::~ExtrudeGeometry() {}

void ExtrudeGeometry::Execute()
{
	START_PROFILE;

	while (HasInput(input_geometry))
	{
		ProceduralObjectPtr in_object = GetInputProceduralObject(input_geometry);
		m_parameterContext()->UpdateExpressions();

		float extrusion_amount = execution_context->parameter_context->GetParameterAs<float>("extrusion_amount");
		Extrusion<Geometry> extrude(extrusion_amount);

		// Geometry
		ProceduralObjectPtr out_object = CreateOutputProceduralObject(output_geometry);
		std::shared_ptr<GeometryComponent> geometry_component = out_object->GetComponent<GeometryComponent>();
		std::shared_ptr<GeometryComponent> in_geometry_component = in_object->GetComponent<GeometryComponent>();
		GeometryPtr in_geometry = in_geometry_component->GetGeometry();

		GeometrySizes sizes = extrude.ResultSize(in_geometry);
		auto out_geometry = std::make_shared<Geometry>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);
		extrude.Execute(in_geometry, out_geometry);

		geometry_component->SetGeometry(out_geometry);

		// Hierarchy
		std::shared_ptr<HierarchicalComponent> in_hierarchical_component =
		    in_object->GetComponent<HierarchicalComponent>();
		std::shared_ptr<HierarchicalComponent> out_hierarchical_component =
		    out_object->GetComponent<HierarchicalComponent>();

		auto hierarchical_system = std::dynamic_pointer_cast<HierarchicalSystem>(
		    execution_context->procedural_object_system->GetComponentSystem(ComponentType::Hierarchical));
		hierarchical_system->SetParent(out_hierarchical_component, in_hierarchical_component);
	}
}
}  // namespace selector
