#include "translate.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/boolean_value.h>
#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/geometry/algorithms/matrix_transform.h>
#include <pagoda/math/matrix_base.h>

#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>

namespace pagoda::geometry::operations {
using namespace objects;
using namespace math;
using namespace geometry::algorithms;
using namespace geometry::core;
using namespace dynamic;

const std::string Translate::s_inputGeometry("in");
const std::string Translate::s_outputGeometry("out");

Translate::Translate(ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem) {}
Translate::~Translate() {}

void Translate::Parameters(objects::NewParameterCallback *cb) {
  cb->FloatParameter(&m_translate.a[0], "x", "X", 0.0f);
  cb->FloatParameter(&m_translate.a[1], "y", "Y", 0.0f);
  cb->FloatParameter(&m_translate.a[2], "z", "Z", 0.0f);
  cb->BooleanParameter(&m_world, "world", "World", false);
}

const std::string &Translate::GetOperationName() const {
  static const std::string sName{"Translate"};
  return sName;
}

void Translate::Interfaces(InterfaceCallback *cb) {
  cb->InputInterface(m_input, s_inputGeometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_output, s_outputGeometry, "Out",
                      Interface::Arity::Many);
}

void Translate::DoWork() {
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
    outGeometryComponent->SetGeometry(outGeometry);

    auto inScope = inGeometryComponent->GetScope();

    Mat4x4F matrix;
    if (m_world) {
      matrix = boost::qvm::translation_mat(m_translate);
    } else {
      matrix = boost::qvm::translation_mat(inScope.GetLocalVector(m_translate));
    }
    MatrixTransform<Geometry> transform(matrix);

    transform.Execute(inGeometry, outGeometry);

    outGeometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
        outGeometry, inScope.GetRotation()));

    m_output->SetNext(outObject);
  }
}
} // namespace pagoda::geometry::operations
