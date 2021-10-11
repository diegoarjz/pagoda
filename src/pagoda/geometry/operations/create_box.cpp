#include "create_box.h"

#include <pagoda/graph/execution_argument_callback.h>

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

void CreateBoxGeometry::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	RegisterMember("xSize", cb->FloatArgument("xSize", "X", 1.0f));
	RegisterMember("ySize", cb->FloatArgument("ySize", "Y", 1.0f));
	RegisterMember("zSize", cb->FloatArgument("zSize", "Z", 1.0f));
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

	float xSize = get_value_as<float>(*GetValue("xSize"));
	float ySize = get_value_as<float>(*GetValue("ySize"));
	float zSize = get_value_as<float>(*GetValue("zSize"));

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	CreateBox<Geometry> createBox(xSize, ySize, zSize);
	auto geometry = std::make_shared<Geometry>();
	createBox.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject();
	std::shared_ptr<GeometryComponent> geometry_component =
	  geometrySystem->CreateComponentAs<GeometryComponent>(object);
	m_output->Set(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  geometry, Mat3x3F(boost::qvm::diag_mat(Vec3F{1.0f, 1.0f, 1.0f}))));
}
}  // namespace pagoda::geometry::operations
