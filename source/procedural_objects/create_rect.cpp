#include "create_rect.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "parameter/parameter.h"
#include "procedural_component.h"

namespace selector
{
const InterfaceName CreateRectGeometry::output_geometry("out", 0);
const char* CreateRectGeometry::name = "CreateRectGeometry";

CreateRectGeometry::CreateRectGeometry()
{
	START_PROFILE;

	ProceduralObjectMask out_geometry_mask;
	out_geometry_mask.set(static_cast<uint32_t>(GeometryComponent::GetType()));
	out_geometry_mask.set(static_cast<uint32_t>(HierarchicalComponent::GetType()));

	CreateOutputInterface(output_geometry, out_geometry_mask);
}

CreateRectGeometry::~CreateRectGeometry() {}

void CreateRectGeometry::Execute()
{
	START_PROFILE;

	float width = execution_context->parameter_context->GetParameterAs<float>("width");
	float height = execution_context->parameter_context->GetParameterAs<float>("height");

	CreateRect<Geometry> create_rect(width, height);
	auto geometry = std::make_shared<Geometry>();
	create_rect.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject(output_geometry);
	std::shared_ptr<GeometryComponent> geometry_component = object->GetComponent<GeometryComponent>();

	geometry_component->SetGeometry(geometry);
}
}  // namespace selector
