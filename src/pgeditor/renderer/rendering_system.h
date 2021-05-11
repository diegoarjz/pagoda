#pragma once

#include "rendering_component.h"

#include "gl/grid_render_task.h"
#include "gl/render_pass.h"

#include <pagoda/objects/procedural_component_system.h>

#include <unordered_set>

namespace pagoda
{
namespace scene
{
using CameraPtr = std::shared_ptr<class Camera>;
}
class Pagoda;
}  // namespace pagoda

namespace pgeditor::renderer
{
/**
 * A \c ProceduralComponentSystem to allow \c ProceduralObject instances to be rendered.
 */
class RenderingSystem : public pagoda::objects::ProceduralComponentSystem<RenderingComponent>
{
	public:
	static const std::string GetComponentSystemName();
	static void Registration(pagoda::Pagoda* pagoda);

	RenderingSystem();
	~RenderingSystem() override {}

	void Render();
	pagoda::scene::CameraPtr& GetCamera();

	protected:
	void DoClone(std::shared_ptr<Component_t> from, std::shared_ptr<Component_t> to) override;

	private:
	std::shared_ptr<gl::GridRenderTask> m_gridTask;
	std::shared_ptr<gl::RenderPass> m_renderPass;
	pagoda::scene::CameraPtr m_camera;

	std::unordered_set<RenderingComponentPtr> m_lastRendered;
};
}  // namespace pgeditor::renderer
