#include "create_rect.h"

#include <pagoda/procedural_objects/geometry_component.h>
#include <pagoda/procedural_objects/geometry_system.h>
#include <pagoda/procedural_objects/hierarchical_component.h>
#include <pagoda/procedural_objects/hierarchical_system.h>
#include <pagoda/procedural_objects/procedural_component.h>
#include <pagoda/procedural_objects/procedural_object_system.h>

#include <pagoda/dynamic_value/float_value.h>
#include <pagoda/dynamic_value/get_value_as.h>

#include <boost/qvm/map_vec_mat.hpp>

namespace pagoda::geometry::operations
{
using namespace geometry::core;
using namespace geometry::algorithms;

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
	math::Vec3F rectXAxis;
	math::Vec3F rectYAxis;

	switch (planeName[0])
	{
		case 'x':
			rectXAxis = math::Vec3F{0, 1, 0};
			rectYAxis = math::Vec3F{0, 0, 1};
			break;
		case 'y':
			rectXAxis = math::Vec3F{1, 0, 0};
			rectYAxis = math::Vec3F{0, 0, -1};
			break;
		case 'z':
			rectXAxis = math::Vec3F{1, 0, 0};
			rectYAxis = math::Vec3F{0, 1, 0};
			break;
		default:
			throw common::exception::Exception(
			    "The 'plane' parameter in create rect must be one of 'x', 'y', or 'z'. It was '" + planeName + "'");
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
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
	    geometry, math::Mat3x3F(boost::qvm::diag_mat(math::Vec3F{1.0f, 1.0f, 1.0f}))));

	std::shared_ptr<HierarchicalComponent> hierarchical_component =
	    hierarchicalSystem->CreateComponentAs<HierarchicalComponent>(object);
}
}  // namespace pagoda::geometry::operations
