#include "create_box.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/algorithms/create_box.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/dynamic/get_value_as.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <boost/qvm/map_vec_mat.hpp>

namespace pagoda::geometry::operations
{
using namespace math;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace objects;
using namespace dynamic;

const std::string CreateBoxGeometry::outputGeometry("out");
const char* CreateBoxGeometry::name = "CreateBoxGeometry";

CreateBoxGeometry::CreateBoxGeometry(ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
	START_PROFILE;
}

CreateBoxGeometry::~CreateBoxGeometry()
{
}

void CreateBoxGeometry::Parameters(objects::NewParameterCallback* cb)
{
	cb->FloatParameter(&m_width, "width", "Width", 1.0f);
	cb->FloatParameter(&m_height, "height", "Height", 1.0f);
	cb->FloatParameter(&m_depth, "depth", "Depth", 1.0f);
}

const std::string& CreateBoxGeometry::GetOperationName() const
{
	static const std::string sName{"CreateBoxGeometry"};
	return sName;
}

void CreateBoxGeometry::Interfaces(InterfaceCallback* cb)
{
	cb->OutputInterface(m_output, outputGeometry, "Out", Interface::Arity::One);
}

void CreateBoxGeometry::DoWork()
{
	START_PROFILE;

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	CreateBox<Geometry> createBox(m_width, m_depth, m_height);
	auto geometry = std::make_shared<Geometry>();
	createBox.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject();
	std::shared_ptr<GeometryComponent> geometry_component =
	  geometrySystem->CreateComponentAs<GeometryComponent>(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  geometry, Mat3x3F(boost::qvm::diag_mat(Vec3F{1.0f, 1.0f, 1.0f}))));

	m_output->Set(object);
}
}  // namespace pagoda::geometry::operations
