#include "render.h"

#include <pgeditor/renderer/rendering_component.h>
#include <pgeditor/renderer/rendering_system.h>

#include <pgeditor/renderer/gl/mesh.h>

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/geometry/geometry_component.h>
#include <pagoda/geometry/geometry_system.h>

#include <pagoda/geometry/algorithms/ear_clipping.h>
#include <pagoda/geometry/core/geometry.h>

#include <pagoda/math/vec_base.h>

using namespace pagoda::math;
using namespace pagoda::objects;
using namespace pagoda::geometry;
using namespace pagoda::geometry::core;
using namespace pagoda::geometry::algorithms;
using namespace pgeditor::renderer::gl;

namespace pgeditor::renderer::operations
{
namespace
{
std::shared_ptr<Mesh> ConvertGeometry(std::shared_ptr<Geometry> pagodaGeom)
{
	std::vector<Mesh::Vertex> verts;
	std::vector<uint32_t> indices;
	for (auto fIter = pagodaGeom->FacesBegin(); fIter != pagodaGeom->FacesEnd();
	     ++fIter) {
		Geometry::FaceAttributes fAttr = pagodaGeom->GetFaceAttributes(*fIter);
		Vec3F normal = fAttr.m_normal;
		for (auto pCirc = pagodaGeom->FacePointCirculatorBegin(*fIter);
		     pCirc.IsValid(); ++pCirc) {
			Geometry::PositionType pos = pagodaGeom->GetPosition(*pCirc);
			Vec2F texCoord = pagodaGeom->GetVertexAttributes(*pCirc).m_texCoords;
			// clang-format off
      Mesh::Vertex v{
        {X(pos), Y(pos), Z(pos)},
        {X(normal), Y(normal), Z(normal)},
        {X(texCoord), Y(texCoord)}
      };
			// clang-format on
			verts.push_back(v);
			indices.push_back(indices.size());
		}
	}
	return std::make_shared<Mesh>(verts, indices);
}
}  // namespace
const std::string Render::input{"in"};
const char* Render::name{"Render"};

Render::Render(pagoda::objects::ProceduralObjectSystemPtr objectSystem)
  : pagoda::objects::ProceduralOperation(objectSystem)
{
}

Render::~Render()
{
}

void Render::SetParameters(pagoda::graph::ExecutionArgumentCallback* cb)
{
}

const std::string& Render::GetOperationName() const
{
	static const std::string kName{"Render"};
	return kName;
}
void Render::Interfaces(pagoda::objects::InterfaceCallback* cb)
{
	cb->InputInterface(m_input, input, "In", Interface::Arity::Many);
}

void Render::DoWork()
{
	START_PROFILE;

	auto geometrySystem =
	  m_proceduralObjectSystem->GetComponentSystem<GeometrySystem>();
	auto renderingSystem =
	  m_proceduralObjectSystem->GetComponentSystem<RenderingSystem>();
	EarClipping<Geometry> earClipping;

	LOG_INFO("Render::DoWork processing input");
	ProceduralObjectPtr inObject = m_input->GetNext();

	auto geometryComponent =
	  geometrySystem->GetComponentAs<GeometryComponent>(inObject);
	auto geometry = geometryComponent->GetGeometry();
	auto triagulatedGeometry = std::make_shared<Geometry>();

	earClipping.Execute(geometry, triagulatedGeometry);

	auto renderingComponent =
	  renderingSystem->CreateComponentAs<RenderingComponent>(inObject);

	renderingComponent->SetMesh(ConvertGeometry(triagulatedGeometry));
}
}  // namespace pgeditor::renderer::operations
