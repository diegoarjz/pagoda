#include "face_offset.h"

#include <pagoda/objects/parameter_callback.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>
#include <pagoda/objects/procedural_component.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/dynamic/float_value.h>
#include <pagoda/dynamic/get_value_as.h>
#include <pagoda/geometry/algorithms/face_offset.h>

#include <memory>

namespace pagoda::geometry::operations {
using namespace objects;
using namespace geometry::core;
using namespace geometry::algorithms;
using namespace dynamic;

const char *FaceOffsetOperation::name = "FaceOffsetOperation";
const std::string FaceOffsetOperation::inputGeometry("in");
const std::string FaceOffsetOperation::outputInnerGeometry("inner");
const std::string FaceOffsetOperation::outputOuterGeometry("outer");

FaceOffsetOperation::FaceOffsetOperation(ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem) {
  START_PROFILE;
}

FaceOffsetOperation::~FaceOffsetOperation() {}

void FaceOffsetOperation::Parameters(objects::NewParameterCallback *cb) {
  cb->FloatParameter(&m_amount, "amount", "Amount", 0.0f);
}

const std::string &FaceOffsetOperation::GetOperationName() const {
  static const std::string sName{"FaceOffset"};
  return sName;
}

void FaceOffsetOperation::Interfaces(InterfaceCallback *cb) {
  cb->InputInterface(m_input, inputGeometry, "In", Interface::Arity::Many);
  cb->OutputInterface(m_inner, outputInnerGeometry, "Inner",
                      Interface::Arity::All);
  cb->OutputInterface(m_outer, outputOuterGeometry, "Outer",
                      Interface::Arity::All);
}

void FaceOffsetOperation::DoWork() {
  START_PROFILE;
  auto geometrySystem =
      m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();

  while (ProceduralObjectPtr inObject = m_input->GetNext()) {

    FaceOffset<Geometry> offset(m_amount);

    std::vector<GeometryPtr> innerGeometries;
    std::vector<GeometryPtr> outerGeometries;

    std::shared_ptr<GeometryComponent> inGeometryComponent =
        geometrySystem->GetComponentAs<GeometryComponent>(inObject);
    GeometryPtr inGeometry = inGeometryComponent->GetGeometry();

    offset.Execute(inGeometry, std::back_inserter(innerGeometries),
                   std::back_inserter(outerGeometries));

    for (const auto &i : innerGeometries) {
      ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject);
      outObject->RegisterOrSetMember("offset_tag",
                                     std::make_shared<String>("inner"));

      std::shared_ptr<GeometryComponent> geometryComponent =
          geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
      geometryComponent->SetGeometry(i);
      geometryComponent->SetScope(Scope::FromGeometryAndConstrainedRotation(
          i, inGeometryComponent->GetScope().GetRotation()));

      m_inner->Add(outObject);
    }

    for (const auto &i : outerGeometries) {
      ProceduralObjectPtr outObject = CreateOutputProceduralObject(inObject);
      outObject->RegisterOrSetMember("offset_tag",
                                     std::make_shared<String>("outer"));

      std::shared_ptr<GeometryComponent> geometryComponent =
          geometrySystem->CreateComponentAs<GeometryComponent>(outObject);
      geometryComponent->SetGeometry(i);
      geometryComponent->SetScope(Scope::FromGeometry(i));

      m_outer->Add(outObject);
    }
  }
} // namespace pagoda::geometry::operations
} // namespace pagoda::geometry::operations
