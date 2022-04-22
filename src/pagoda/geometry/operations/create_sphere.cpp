#include "create_sphere.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/algorithms/create_sphere.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/dynamic/get_value_as.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <boost/qvm/map_vec_mat.hpp>

#include <pagoda/scene/nodes/geometry.h>
#include <pagoda/scene/path.h>
#include <pagoda/scene/scene_graph.h>

namespace pagoda::geometry::operations {
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;
using namespace scene;

const std::string CreateSphereGeometry::outputGeometry("out");
const char *CreateSphereGeometry::name = "CreateSphereGeometry";

CreateSphereGeometry::CreateSphereGeometry(
    ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem) {
  START_PROFILE;
}

CreateSphereGeometry::~CreateSphereGeometry() {}

void CreateSphereGeometry::Parameters(objects::NewParameterCallback *cb) {
  cb->FloatParameter(&m_radius, "radius", "Radius", 1.0f);
  cb->IntegerParameter(&m_slices, "slices", "Slices", 10);
  cb->IntegerParameter(&m_stacks, "stacks", "Stacks", 10);
}

const std::string &CreateSphereGeometry::GetOperationName() const {
  static const std::string sName{"CreateSphereGeometry"};
  return sName;
}

void CreateSphereGeometry::Interfaces(InterfaceCallback *cb) {
  cb->OutputInterface(m_output, outputGeometry, "Out", Interface::Arity::One);
}

void CreateSphereGeometry::DoWork() {
  START_PROFILE;

  auto geometrySystem =
      m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

  CreateSphere<Geometry> createSphere(m_radius, m_slices, m_stacks);
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

  m_proceduralObjectSystem->GetSceneGraph()
      ->CreateNode<nodes::Geometry>(Path{name})
      ->SetGeometry(geometry);
}
} // namespace pagoda::geometry::operations
