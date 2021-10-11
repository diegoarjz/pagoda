#include "create_sphere.h"

#include <pagoda/graph/execution_argument_callback.h>

#include <pagoda/geometry/algorithms/create_sphere.h>
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
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;

const std::string CreateSphereGeometry::outputGeometry("out");
const char* CreateSphereGeometry::name = "CreateSphereGeometry";

CreateSphereGeometry::CreateSphereGeometry(
  ProceduralObjectSystemPtr objectSystem)
  : ProceduralOperation(objectSystem)
{
	START_PROFILE;
}

CreateSphereGeometry::~CreateSphereGeometry()
{
}

void CreateSphereGeometry::SetParameters(graph::ExecutionArgumentCallback* cb)
{
	RegisterMember("radius", cb->FloatArgument("radius", "Radius", 1.0f));
	RegisterMember("slices", cb->IntegerArgument("slices", "Slices", 10));
	RegisterMember("stacks", cb->IntegerArgument("stacks", "Stacks", 10));
}

const std::string& CreateSphereGeometry::GetOperationName() const
{
	static const std::string sName{"CreateSphereGeometry"};
	return sName;
}

void CreateSphereGeometry::Interfaces(InterfaceCallback* cb)
{
	cb->OutputInterface(m_output, outputGeometry, "Out", Interface::Arity::One);
}

void CreateSphereGeometry::DoWork()
{
	START_PROFILE;

	float radius = get_value_as<float>(*GetValue("radius"));
	int slices = get_value_as<int>(*GetValue("slices"));
	int stacks = get_value_as<int>(*GetValue("stacks"));

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

	CreateSphere<Geometry> createSphere(radius, slices, stacks);
	auto geometry = std::make_shared<Geometry>();
	createSphere.Execute(geometry);

	ProceduralObjectPtr object = CreateOutputProceduralObject();
	std::shared_ptr<GeometryComponent> geometry_component =
	  geometrySystem->CreateComponentAs<GeometryComponent>(object);
	m_output->Set(object);

	geometry_component->SetGeometry(geometry);
	geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
	  geometry,
	  math::Mat3x3F(boost::qvm::diag_mat(math::Vec3F{1.0f, 1.0f, 1.0f}))));
}
}  // namespace pagoda::geometry::operations
