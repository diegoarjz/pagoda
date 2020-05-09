#include "create_sphere.h"

#include "dynamic_value/get_value_as.h"
#include "geometry_component.h"
#include "geometry_operations/create_sphere.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_component.h"
#include "procedural_object_system.h"

#include <boost/qvm/map_vec_mat.hpp>

namespace pagoda
{
const std::string CreateSphereGeometry::outputGeometry("out");
const char* CreateSphereGeometry::name = "CreateSphereGeometry";

CreateSphereGeometry::CreateSphereGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	RegisterValues({{"radius", std::make_shared<FloatValue>(1.0f)},
	                {"slices", std::make_shared<Integer>(10)},
	                {"stacks", std::make_shared<Integer>(10)}});

	CreateOutputInterface(outputGeometry);
}

CreateSphereGeometry::~CreateSphereGeometry() {}

void CreateSphereGeometry::DoWork()
{
	START_PROFILE;

	float radius = get_value_as<float>(*GetValue("radius"));
	int slices = get_value_as<int>(*GetValue("slices"));
	int stacks = get_value_as<int>(*GetValue("stacks"));

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	CreateSphere<Geometry> createSphere(radius, slices, stacks);
	auto geometry = std::make_shared<Geometry>();
	createSphere.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject(outputGeometry);
	std::shared_ptr<GeometryComponent> geometry_component =
	    geometrySystem->CreateComponentAs<GeometryComponent>(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(
	    Scope::FromGeometryAndConstrainedRotation(geometry, Mat3x3F(boost::qvm::diag_mat(Vec3F{1.0f, 1.0f, 1.0f}))));

	std::shared_ptr<HierarchicalComponent> hierarchical_component =
	    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
}
}  // namespace pagoda
