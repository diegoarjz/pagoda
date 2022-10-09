#include "clip_geometry.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/geometry/algorithms/clip.h>

namespace pagoda::geometry::operations {
using namespace objects;
using namespace math;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;

const std::string ClipGeometry::inputGeometry("in");
const std::string ClipGeometry::frontGeometry("front");
const std::string ClipGeometry::backGeometry("back");
const char *ClipGeometry::name = "ClipGeometry";

ClipGeometry::ClipGeometry(objects::ProceduralObjectSystemPtr objectSystem)
    : objects::ProceduralOperation(objectSystem) {
  START_PROFILE;
}

ClipGeometry::~ClipGeometry() {}

void ClipGeometry::Parameters(objects::NewParameterCallback *cb) {
  cb->PlaneParameter(&m_plane, "plane", "Clip Plane");
}

const std::string &ClipGeometry::GetOperationName() const {
  static const std::string sName{"ClipGeometry"};
  return sName;
}

void ClipGeometry::Interfaces(InterfaceCallback *cb) {
  cb->InputInterface(m_inputGeometry, inputGeometry, "In",
                     Interface::Arity::Many);
  cb->OutputInterface(m_frontGeometry, frontGeometry, "Front",
                      Interface::Arity::Many);
  cb->OutputInterface(m_backGeometry, backGeometry, "Back",
                      Interface::Arity::Many);
}

void ClipGeometry::DoWork() {
  START_PROFILE;

  auto geometrySystem =
      m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

  while (ProceduralObjectPtr inObject = m_inputGeometry->GetNext()) {

    Clip<Geometry> clip(m_plane);

    // Geometry
    auto inGeometryComponent =
        geometrySystem->GetComponentAs<GeometryComponent>(inObject);
    GeometryPtr inGeometry = inGeometryComponent->GetGeometry();

    auto frontProceduralObject = CreateOutputProceduralObject(inObject);
    auto frontGeometryComponent =
        geometrySystem->CreateComponentAs<GeometryComponent>(
            frontProceduralObject);
    auto front = std::make_shared<Geometry>();
    frontGeometryComponent->SetGeometry(front);
    m_frontGeometry->SetNext(frontProceduralObject);

    auto backProceduralObject = CreateOutputProceduralObject(inObject);
    auto backGeometryComponent =
        geometrySystem->CreateComponentAs<GeometryComponent>(
            backProceduralObject);
    auto back = std::make_shared<Geometry>();
    backGeometryComponent->SetGeometry(back);
    m_backGeometry->SetNext(backProceduralObject);

    clip.Execute(inGeometry, front, back);

    frontGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
        front, inGeometryComponent->GetScope().GetRotation()));
    backGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
        back, inGeometryComponent->GetScope().GetRotation()));
  }
}
} // namespace pagoda::geometry::operations
