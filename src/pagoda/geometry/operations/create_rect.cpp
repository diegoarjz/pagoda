#include "create_rect.h"

#include <pagoda/objects/parameter_callback.h>

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

CreateRectGeometry::CreateRectGeometry(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
	START_PROFILE;
}

CreateRectGeometry::~CreateRectGeometry()
{
}

void CreateRectGeometry::Parameters(objects::NewParameterCallback* cb)
{
	cb->FloatParameter(&m_width, "width", "Width", 1.0f);
	cb->FloatParameter(&m_height, "height", "Height", 1.0f);
	cb->StringParameter(&m_plane, "plane", "Plane", "z");
}

const std::string& CreateRectGeometry::GetOperationName() const
{
	static const std::string sName{"CreateRectGeometry"};
	return sName;
}

void CreateRectGeometry::Interfaces(InterfaceCallback* cb)
{
	cb->OutputInterface(m_out, output_geometry, "Out", Interface::Arity::One);
}

void CreateRectGeometry::DoWork()
{
	START_PROFILE;

	std::cout << "Creating rect " << m_width << " " << m_height << " " << m_plane
	          << std::endl;

	math::Vec3F rectXAxis;
	math::Vec3F rectYAxis;
	switch (m_plane[0]) {
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
			  "The 'plane' parameter in create rect must be one of 'x', 'y', or 'z'. "
			  "It was '" +
			  m_plane + "'");
	}

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	CreateRect<Geometry> create_rect(m_width, m_height, rectXAxis, rectYAxis);
	auto geometry = std::make_shared<Geometry>();
	create_rect.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject();
	std::shared_ptr<GeometryComponent> geometry_component =
	  geometrySystem->CreateComponentAs<GeometryComponent>(object);
	m_out->Set(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  geometry,
	  math::Mat3x3F(boost::qvm::diag_mat(math::Vec3F{1.0f, 1.0f, 1.0f}))));
}
}  // namespace pagoda::geometry::operations
