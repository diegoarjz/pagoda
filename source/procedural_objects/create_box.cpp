#include "create_box.h"

#include "geometry_component.h"
#include "geometry_operations/create_box.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "parameter/parameter.h"
#include "procedural_component.h"
#include "procedural_object_system.h"

namespace selector
{
const InterfaceName CreateBoxGeometry::outputGeometry("out", 0);
const char* CreateBoxGeometry::name = "CreateBoxGeometry";

CreateBoxGeometry::CreateBoxGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;
	RegisterMember("xSize", std::make_shared<FloatValue>(0.0f));
	RegisterMember("ySize", std::make_shared<FloatValue>(0.0f));
	RegisterMember("zSize", std::make_shared<FloatValue>(0.0f));

	CreateOutputInterface(outputGeometry);
}

CreateBoxGeometry::~CreateBoxGeometry() {}

void CreateBoxGeometry::Execute()
{
	START_PROFILE;

	float xSize = get_parameter_as<float>(GetMember("xSize"));
	float ySize = get_parameter_as<float>(GetMember("ySize"));
	float zSize = get_parameter_as<float>(GetMember("zSize"));

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	CreateBox<Geometry> createBox(xSize, ySize, zSize);
	auto geometry = std::make_shared<Geometry>();
	createBox.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject(outputGeometry);
	std::shared_ptr<GeometryComponent> geometry_component =
	    geometrySystem->CreateComponentAs<GeometryComponent>(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(geometry, Mat3x3F(1)));

	std::shared_ptr<HierarchicalComponent> hierarchical_component =
	    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
}
}  // namespace selector
