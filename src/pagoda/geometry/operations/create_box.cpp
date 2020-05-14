#include "create_box.h"

#include <pagoda/geometry/algorithms/create_box.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/dynamic_value/get_value_as.h>
#include <pagoda/objects/hierarchical_component.h>
#include <pagoda/objects/hierarchical_system.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <boost/qvm/map_vec_mat.hpp>

namespace pagoda::geometry::operations
{
using namespace math;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace objects;

const std::string CreateBoxGeometry::outputGeometry("out");
const char* CreateBoxGeometry::name = "CreateBoxGeometry";

CreateBoxGeometry::CreateBoxGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;
	RegisterValues({{"xSize", std::make_shared<FloatValue>(0.0f)},
	                {"ySize", std::make_shared<FloatValue>(0.0f)},
	                {"zSize", std::make_shared<FloatValue>(0.0f)}});

	CreateOutputInterface(outputGeometry);
}

CreateBoxGeometry::~CreateBoxGeometry() {}

void CreateBoxGeometry::DoWork()
{
	START_PROFILE;

	float xSize = get_value_as<float>(*GetValue("xSize"));
	float ySize = get_value_as<float>(*GetValue("ySize"));
	float zSize = get_value_as<float>(*GetValue("zSize"));

	auto geometrySystem = m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto hierarchicalSystem = m_proceduralObjectSystem->GetComponentSystem<HierarchicalSystem>();

	CreateBox<Geometry> createBox(xSize, ySize, zSize);
	auto geometry = std::make_shared<Geometry>();
	createBox.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject(outputGeometry);
	std::shared_ptr<GeometryComponent> geometry_component =
	    geometrySystem->CreateComponentAs<GeometryComponent>(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(
	    Scope::FromGeometryAndConstrainedRotation(geometry, Mat3x3F(boost::qvm::diag_mat(Vec3F{1.0f, 1.0f, 1.0f}))));

	std::shared_ptr<HierarchicalComponent> hierarchical_component =
	    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
}
}  // namespace pagoda::geometry::operations
