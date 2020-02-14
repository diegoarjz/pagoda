#include "create_rect.h"

#include "dynamic_value/float_value.h"
#include "dynamic_value/get_value_as.h"
#include "geometry_component.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_component.h"
#include "procedural_object_system.h"

namespace selector
{
const std::string CreateRectGeometry::output_geometry("out");
const char* CreateRectGeometry::name = "CreateRectGeometry";

CreateRectGeometry::CreateRectGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	RegisterValues({{"width", std::make_shared<FloatValue>(0.0f)},
	                {"height", std::make_shared<FloatValue>(0.0f)},
	                {"plane", std::make_shared<String>("z")}});

	CreateOutputInterface(output_geometry);
}

CreateRectGeometry::~CreateRectGeometry() {}

void CreateRectGeometry::DoWork()
{
	START_PROFILE;

	float width = get_value_as<float>(*GetValue("width"));
	float height = get_value_as<float>(*GetValue("height"));
	std::string planeName = get_value_as<std::string>(*GetValue("plane"));
	Vec3F rectXAxis;
	Vec3F rectYAxis;

	switch (planeName[0])
	{
		case 'x':
			rectXAxis = Vec3F(0, 1, 0);
			rectYAxis = Vec3F(0, 0, 1);
			break;
		case 'y':
			rectXAxis = Vec3F(1, 0, 0);
			rectYAxis = Vec3F(0, 0, -1);
			break;
		case 'z':
			rectXAxis = Vec3F(1, 0, 0);
			rectYAxis = Vec3F(0, 1, 0);
			break;
		default:
			throw Exception("The 'plane' parameter in create rect must be one of 'x', 'y', or 'z'. It was '" +
			                planeName + "'");
	}

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	CreateRect<Geometry> create_rect(width, height, rectXAxis, rectYAxis);
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
