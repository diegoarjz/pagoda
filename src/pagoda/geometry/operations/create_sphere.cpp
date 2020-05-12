#include "create_sphere.h"

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_component.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

#include <pagoda/dynamic_value/get_value_as.h>
#include <pagoda/geometry/algorithms/create_sphere.h>

#include <boost/qvm/map_vec_mat.hpp>

namespace pagoda::geometry::operations
{
using namespace geometry::core;
using namespace geometry::algorithms;

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
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
	    geometry, math::Mat3x3F(boost::qvm::diag_mat(math::Vec3F{1.0f, 1.0f, 1.0f}))));

	std::shared_ptr<HierarchicalComponent> hierarchical_component =
	    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
}
}  // namespace pagoda::geometry::operations
