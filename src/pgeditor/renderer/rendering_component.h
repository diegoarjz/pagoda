#pragma once

#include <pagoda/objects/procedural_component.h>

namespace pgeditor::renderer
{
namespace gl
{
using MeshPtr = std::shared_ptr<class Mesh>;
}

/**
 * A RenderingComponent for ProceduralObject.
 */
class RenderingComponent : public pagoda::objects::ProceduralComponent
{
	public:
	static std::string GetComponentSystemName();

	std::string GetType() const override { return GetComponentSystemName(); }

	~RenderingComponent() override {}

	void SetMesh(pgeditor::renderer::gl::MeshPtr mesh);
	const pgeditor::renderer::gl::MeshPtr& GetMesh() const;

	private:
	pgeditor::renderer::gl::MeshPtr m_mesh;
};
}  // namespace pgeditor::renderer
