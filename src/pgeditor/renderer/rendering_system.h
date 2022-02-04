#pragma once

#include "rendering_component.h"
#include "renderer.h"

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

  void SetRenderer(const std::shared_ptr<Renderer>& renderer);

	void Render();
	pagoda::scene::CameraPtr& GetCamera();

	protected:
	void DoClone(std::shared_ptr<Component_t> from, std::shared_ptr<Component_t> to) override;

	private:
	pagoda::scene::CameraPtr m_camera;

  std::shared_ptr<Renderer> m_renderer;
	std::unordered_set<RenderingComponentPtr> m_lastRendered;
};
}  // namespace pgeditor::renderer
