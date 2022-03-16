#include "render.h"

#include <pgeditor/renderer/renderable.h>
#include <pgeditor/renderer/rendering_component.h>
#include <pgeditor/renderer/rendering_system.h>

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/geometry/algorithms/ear_clipping.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/material/material_component.h>
#include <pagoda/material/material_system.h>

#include <pagoda/math/vec_base.h>

using namespace pagoda::math;
using namespace pagoda::objects;
using namespace pagoda::geometry;
using namespace pagoda::geometry::core;
using namespace pagoda::geometry::algorithms;
using namespace pagoda::material;

namespace pgeditor::renderer::operations {
namespace {
std::shared_ptr<Renderable>
ConvertGeometry(std::shared_ptr<Geometry> pagodaGeom) {
  RenderablePtr renderable = std::make_shared<Renderable>();

  std::vector<Vec4F> points;
  std::vector<Vec3F> normals;
  std::vector<uint32_t> indices;

  for (auto fIter = pagodaGeom->FacesBegin(); fIter != pagodaGeom->FacesEnd();
       ++fIter) {
    Geometry::FaceAttributes fAttr = pagodaGeom->GetFaceAttributes(*fIter);
    Vec3F normal = fAttr.m_normal;
    for (auto pCirc = pagodaGeom->FacePointCirculatorBegin(*fIter);
         pCirc.IsValid(); ++pCirc) {
      Geometry::PositionType pos = pagodaGeom->GetPosition(*pCirc);

      points.push_back(Vec4F{X(pos), Y(pos), Z(pos), 1.0f});
      normals.push_back(Vec3F{X(normal), Y(normal), Z(normal)});
      indices.push_back(indices.size());
    }
  }

  renderable->GetBuffer("position")->SetData(points);
  renderable->GetBuffer("normal")->SetData(normals);
  renderable->GetIndexBuffer()->SetData(indices);

  return renderable;
}
} // namespace

const std::string Render::input{"in"};
const char *Render::name{"Render"};

Render::Render(pagoda::objects::ProceduralObjectSystemPtr objectSystem)
    : pagoda::objects::ProceduralOperation(objectSystem) {}

Render::~Render() {}

void Render::SetParameters(pagoda::objects::ParameterCallback *cb) {}

const std::string &Render::GetOperationName() const {
  static const std::string kName{"Render"};
  return kName;
}
void Render::Interfaces(pagoda::objects::InterfaceCallback *cb) {
  cb->InputInterface(m_input, input, "In", Interface::Arity::Many);
}

void Render::DoWork() {
  START_PROFILE;

  auto geometrySystem =
      m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
  auto renderingSystem =
      m_proceduralObjectSystem->GetComponentSystem<RenderingSystem>();
  auto materialSystem =
      m_proceduralObjectSystem->GetComponentSystem<MaterialSystem>();
  EarClipping<Geometry> earClipping;

  LOG_INFO("Render::DoWork processing input");
  ProceduralObjectPtr inObject = m_input->GetNext();

  auto geometryComponent =
      geometrySystem->GetComponentAs<GeometryComponent>(inObject);
  auto geometry = geometryComponent->GetGeometry();
  auto triagulatedGeometry = std::make_shared<Geometry>();

  auto materialComponent = materialSystem->GetComponentAs<MaterialComponent>(inObject);

  earClipping.Execute(geometry, triagulatedGeometry);

  auto renderingComponent =
      renderingSystem->CreateComponentAs<RenderingComponent>(inObject);

  RenderablePtr renderable = ConvertGeometry(triagulatedGeometry);
  renderable->SetMaterial(materialComponent->GetMaterial());
  renderingComponent->SetRenderable(renderable);
}
} // namespace pgeditor::renderer::operations
