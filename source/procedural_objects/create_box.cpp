#include "create_box.h"

#include "dynamic_value/get_value_as.h"
#include "geometry_component.h"
#include "geometry_operations/create_box.h"
#include "geometry_system.h"
#include "hierarchical_component.h"
#include "hierarchical_system.h"
#include "procedural_component.h"
#include "procedural_object_system.h"

#include <boost/qvm/map_vec_mat.hpp>

namespace selector
{
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
}  // namespace selector
