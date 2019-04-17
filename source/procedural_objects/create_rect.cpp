#include "create_rect.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "parameter/float_parameter.h"
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

	float w = width->Get<FloatParameter>();
	float h = height->Get<FloatParameter>();

	CreateRect<Geometry> create_rect(w, h);
	GeometrySizes sizes = create_rect.ResultSize();
	auto geometry = std::make_shared<Geometry>(sizes.m_numVertices, sizes.m_numEdges, sizes.m_numFaces);
	create_rect.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject(output_geometry);
	std::shared_ptr<GeometryComponent> geometry_component = object->GetComponent<GeometryComponent>();

	geometry_component->SetGeometry(geometry);
}

void CreateRectGeometry::SetExecutionContext(std::shared_ptr<OperationExecutionContext> context)
{
	START_PROFILE;

	ProceduralOperation::SetExecutionContext(context);

	width = context->parameter_context->GetParameterAs<FloatParameter>("width");
	height = context->parameter_context->GetParameterAs<FloatParameter>("height");

	DBG_ASSERT_MSG(width != nullptr && height != nullptr, "width or height parameter not found in context");
}

std::shared_ptr<ParameterBase> CreateRectGeometry::GetParameter(const std::string& name)
{
	START_PROFILE;

	DBG_ASSERT_MSG(name == "width" || name == "height", "Unknown parameter");
	if (name == "width")
	{
		return width;
	}
	else
	{
		return height;
	}
}
}  // namespace selector
