#include "render_pass.h"

#include "default_material.h"
#include "mesh.h"

#include <pgeditor/renderer/rendering_component.h>

#include <boost/qvm/mat_operations.hpp>

namespace pgeditor::renderer::gl
{
void RenderPass::Load()
{
	m_material = std::make_shared<DefaultMaterial>();
	m_material->Load();
}

void RenderPass::SetUp() { glEnable(GL_DEPTH_TEST); }

void RenderPass::Render(std::vector<RenderingComponentPtr>& objects)
{
	m_material->SetProjectionMatrix(m_projectionMatrix);
	m_material->SetViewMatrix(m_viewMatrix);
	m_material->SetModelMatrix(boost::qvm::identity_mat<float, 4>());

	m_material->Render();
	for (const auto& o : objects) {
		o->GetMesh()->Render();
	}
}

void RenderPass::TearDown()
{
	//
}

void RenderPass::Dispose()
{
	m_material->Dispose();
	m_material.reset();
}

void RenderPass::SetProjectionMatrix(const pagoda::math::Mat4x4F& mat) { m_projectionMatrix = mat; }

void RenderPass::SetViewMatrix(const pagoda::math::Mat4x4F& mat) { m_viewMatrix = mat; }

}  // namespace pgeditor::renderer::gl
