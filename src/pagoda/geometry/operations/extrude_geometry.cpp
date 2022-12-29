#include "extrude_geometry.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/algorithms/extrusion.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>

#include <memory>

namespace pagoda::geometry::operations {
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;

const char *ExtrudeGeometry::name = "ExtrudeGeometry";
const std::string ExtrudeGeometry::input_geometry("in");
const std::string ExtrudeGeometry::output_geometry("out");

ExtrudeGeometry::ExtrudeGeometry(ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem) {
  START_PROFILE;
}

ExtrudeGeometry::~ExtrudeGeometry() {}

void ExtrudeGeometry::Parameters(objects::NewParameterCallback *cb) {
  cb->FloatParameter(&m_amount, "amount", "Amount", 1.0f);
}

const std::string &ExtrudeGeometry::GetOperationName() const {
  static const std::string sName{"ExtrudeGeometry"};
  return sName;
}

void ExtrudeGeometry::Interfaces(InterfaceCallback *cb) {
  cb->InputInterface(m_in, input_geometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_out, output_geometry, "Out", Interface::Arity::Many);
}

void ExtrudeGeometry::DoWork() {
  START_PROFILE;
  auto geometrySystem =
      m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

  while (ProceduralObjectPtr in_object = m_in->GetNext()) {

    Extrusion<Geometry> extrude(m_amount);

    // Geometry
    ProceduralObjectPtr out_object = CreateOutputProceduralObject(in_object);
    std::shared_ptr<GeometryComponent> geometry_component =
        geometrySystem->CreateComponentAs<GeometryComponent>(out_object);
    std::shared_ptr<GeometryComponent> in_geometry_component =
        geometrySystem->GetComponentAs<GeometryComponent>(in_object);
    GeometryPtr in_geometry = in_geometry_component->GetGeometry();

    auto out_geometry = std::make_shared<Geometry>();
    extrude.Execute(in_geometry, out_geometry);

    geometry_component->SetGeometry(out_geometry);
    geometry_component->SetScope(Scope::FromGeometryAndConstrainedRotation(
        out_geometry, in_geometry_component->GetScope().GetRotation()));

    m_out->SetNext(out_object);
  }
}
} // namespace pagoda::geometry::operations
