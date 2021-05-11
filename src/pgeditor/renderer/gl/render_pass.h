#pragma once

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <vector>

namespace pgeditor::renderer
{
using RenderingComponentPtr = std::shared_ptr<class RenderingComponent>;
}

namespace pgeditor::renderer::gl
{
using DefaultMaterialPtr = std::shared_ptr<class DefaultMaterial>;

class RenderPass
{
	public:
	void Load();
	void SetUp();
	void Render(std::vector<RenderingComponentPtr> &objects);
	void TearDown();
	void Dispose();

	void SetProjectionMatrix(const pagoda::math::Mat4x4F &mat);
	void SetViewMatrix(const pagoda::math::Mat4x4F &mat);

	private:
	DefaultMaterialPtr m_material;
	pagoda::math::Mat4x4F m_projectionMatrix;
	pagoda::math::Mat4x4F m_viewMatrix;
};
}  // namespace pgeditor::renderer::gl
