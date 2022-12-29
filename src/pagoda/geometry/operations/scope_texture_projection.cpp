#include "scope_texture_projection.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/core/planar_texture_projection.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/dynamic/string_value.h>

#include <memory>

using namespace boost;

namespace pagoda::geometry::operations {
using namespace objects;
using namespace math;
using namespace geometry::core;
using namespace dynamic;

const std::string ScopeTextureProjection::s_inputGeometry("in");
const std::string ScopeTextureProjection::s_outputGeometry("out");

ScopeTextureProjection::ScopeTextureProjection(
    objects::ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem) {}

ScopeTextureProjection::~ScopeTextureProjection() {}

void ScopeTextureProjection::Parameters(objects::NewParameterCallback *cb) {
  cb->StringParameter(&m_axis, "axis", "Axis", "z");
  cb->FloatParameter(&m_scale.a[0], "scale_u", "Scale U", 1.0f);
  cb->FloatParameter(&m_scale.a[1], "scale_v", "Scale V", 1.0f);
  cb->FloatParameter(&m_offset.a[0], "offset_u", "Offset U", 0.0f);
  cb->FloatParameter(&m_offset.a[1], "offset_v", "Offset V", 0.0f);
  cb->BooleanParameter(&m_clamp, "clamp", "Clamp", false);
}

const std::string &ScopeTextureProjection::GetOperationName() const {
  static const std::string sName{"ScopeTextureProjection"};
  return sName;
}

void ScopeTextureProjection::Interfaces(InterfaceCallback *cb) {
  cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_output, s_outputGeometry, "Out",
                      Interface::Arity::Many);
}

void ScopeTextureProjection::DoWork() {
  START_PROFILE;

  auto geometrySystem =
      m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

  while (ProceduralObjectPtr inObject = m_input->GetNext()) {
    ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject);

    auto inGeometryComponent =
        geometrySystem->GetComponentAs<GeometryComponent>(inObject);
    GeometryPtr inGeometry = inGeometryComponent->GetGeometry();
    auto outGeometryComponent =
        geometrySystem->CreateComponentAs<GeometryComponent>(outObject);

    auto outGeometry = std::make_shared<Geometry>();
    *outGeometry = *inGeometry;
    outGeometryComponent->SetGeometry(outGeometry);
    outGeometryComponent->SetScope(inGeometryComponent->GetScope());

    auto &scope = outGeometryComponent->GetScope();
    auto scopeSize = scope.GetSize();
    qvm::vec<float, 3> scopePos = scope.GetPosition();
    qvm::vec<float, 3> uAxis;
    qvm::vec<float, 3> vAxis;

    if (m_axis == "x") {
      uAxis = scope.GetAxis('y');
      vAxis = scope.GetAxis('z');
      m_scale.a[0] *= Y(scopeSize);
      m_scale.a[1] *= Z(scopeSize);
    } else if (m_axis == "y") {
      uAxis = scope.GetAxis('x');
      vAxis = scope.GetAxis('z');
      m_scale.a[0] *= X(scopeSize);
      m_scale.a[1] *= Z(scopeSize);
    } else if (m_axis == "z") {
      uAxis = scope.GetAxis('x');
      vAxis = scope.GetAxis('y');
      m_scale.a[0] *= X(scopeSize);
      m_scale.a[1] *= Y(scopeSize);
    }

    PlanarTextureProjection ptp(scopePos, uAxis, vAxis);
    ptp.SetScale(m_scale.a[0], m_scale.a[1]);
    ptp.SetOffset(m_offset.a[0], m_offset.a[1]);
    ptp.SetClamp(m_clamp);

    for (auto i = outGeometry->PointsBegin(); i != outGeometry->PointsEnd();
         ++i) {
      outGeometry->GetVertexAttributes(*i).m_texCoords =
          ptp.GetProjection(outGeometry->GetPosition(*i));
    }

    m_output->SetNext(outObject);
  }
}
} // namespace pagoda::geometry::operations
