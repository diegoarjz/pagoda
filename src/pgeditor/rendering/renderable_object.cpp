#include "renderable_object.h"

#include <pagoda/common/debug/logger.h>

#include "geometry.h"
#include "material.h"
#include "shader_program.h"
#include "uniform.h"

#include "vertex.h"

using namespace pagoda::math;

namespace pgeditor::rendering
{
RenderableObject::RenderableObject(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
    : m_geometry(geometry), m_material(material)
{
}
RenderableObject::~RenderableObject() {}

std::shared_ptr<Geometry> RenderableObject::GetGeometry() const { return m_geometry; }
std::shared_ptr<Material> RenderableObject::GetMaterial() const { return m_material; }

void RenderableObject::SetModelMatrix(const boost::qvm::mat<float, 4, 4>& modelMatrix) { m_modelMatrix = modelMatrix; }

const boost::qvm::mat<float, 4, 4>& RenderableObject::GetModelMatrix() const { return m_modelMatrix; }

void RenderableObject::SetViewMatrix(const boost::qvm::mat<float, 4, 4>& viewMatrix) { m_viewMatrix = viewMatrix; }
const boost::qvm::mat<float, 4, 4>& RenderableObject::GetViewMatrix() const { return m_viewMatrix; }

void RenderableObject::SetProjectionMatrix(const boost::qvm::mat<float, 4, 4>& projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;
}

const boost::qvm::mat<float, 4, 4>& RenderableObject::GetProjectionMatrix() const { return m_projectionMatrix; }

void RenderableObject::DoLoad(Renderer* r)
{
	LOG_DEBUG("Loading RenderableObject");
	m_geometry->Load(r);
	m_material->Load(r);
}

void RenderableObject::DoRender(Renderer* r)
{
	m_material->Render(r);
	for (auto& uniform : m_material->GetUniforms())
	{
		switch (uniform->GetSemantics())
		{
			case Uniform::Semantics::ModelMatrix:
				uniform->SetMatrix4(GetModelMatrix());
				break;
			case Uniform::Semantics::ViewMatrix:
				uniform->SetMatrix4(GetViewMatrix());
				break;
			case Uniform::Semantics::ProjectionMatrix:
				uniform->SetMatrix4(GetProjectionMatrix());
				break;
			default:
				break;
		};
	}

	m_geometry->Render(r);
}

void RenderableObject::DoDispose(Renderer* r)
{
	//
}

}  // namespace pgeditor::rendering
