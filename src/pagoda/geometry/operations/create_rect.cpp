#include "create_rect.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/geometry/algorithms/create_rect.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>

#include <boost/qvm/map_vec_mat.hpp>

using namespace pagoda::objects;

namespace pagoda::geometry::operations
{
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;

const std::string CreateRectGeometry::output_geometry("out");
const char* CreateRectGeometry::name = "CreateRectGeometry";

CreateRectGeometry::CreateRectGeometry(ProceduralObjectSystemPtr objectSystem) : ProceduralOperation(objectSystem)
{
	START_PROFILE;

	CreateOutputInterface(output_geometry);
}

CreateRectGeometry::~CreateRectGeometry() {}

void CreateRectGeometry::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	RegisterMember("width", cb->FloatArgument("width", "Width", 1.0f));
	RegisterMember("height", cb->FloatArgument("height", "Height", 1.0f));
	RegisterMember("plane", cb->StringArgument("plane", "Plane", "z"));
}

const std::string& CreateRectGeometry::GetOperationName() const
{
	static const std::string sName{"CreateRectGeometry"};
	return sName;
}

void CreateRectGeometry::Interfaces(InterfaceCallback* cb)
{
	cb->OutputInterface(m_out, output_geometry, "Out", Interface::Arity::All);
}

void CreateRectGeometry::DoWork()
{
	START_PROFILE;

	float width = get_value_as<float>(*GetValue("width"));
	float height = get_value_as<float>(*GetValue("height"));
	std::string planeName = get_value_as<std::string>(*GetValue("plane"));
	math::Vec3F rectXAxis;
	math::Vec3F rectYAxis;

	switch (planeName[0]) {
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

	CreateRect<Geometry> create_rect(width, height, rectXAxis, rectYAxis);
	auto geometry = std::make_shared<Geometry>();
	create_rect.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject(output_geometry);
	std::shared_ptr<GeometryComponent> geometry_component = geometrySystem->CreateComponentAs<GeometryComponent>(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  geometry, math::Mat3x3F(boost::qvm::diag_mat(math::Vec3F{1.0f, 1.0f, 1.0f}))));
}
}  // namespace pagoda::geometry::operations
