#include "create_rect.h"

#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "parameter/parameter.h"
#include "procedural_component.h"
#include "procedural_object_system.h"

namespace selector
{
const InterfaceName CreateRectGeometry::output_geometry("out", 0);
const char* CreateRectGeometry::name = "CreateRectGeometry";

CreateRectGeometry::CreateRectGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	SetParameter("width", 0.0f);
	SetParameter("height", 0.0f);

	CreateOutputInterface(output_geometry);
}

CreateRectGeometry::~CreateRectGeometry() {}

void CreateRectGeometry::Execute()
{
	START_PROFILE;

	float width = GetParameterAs<float>("width");
	float height = GetParameterAs<float>("height");

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	CreateRect<Geometry> create_rect(width, height);
	auto geometry = std::make_shared<Geometry>();
	create_rect.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject(output_geometry);
	std::shared_ptr<GeometryComponent> geometry_component =
	    geometrySystem->CreateComponentAs<GeometryComponent>(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(geometry, Mat3x3F(1)));

	std::shared_ptr<HierarchicalComponent> hierarchical_component =
	    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
}
}  // namespace selector
