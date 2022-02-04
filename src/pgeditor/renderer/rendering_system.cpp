#include "rendering_system.h"

#include "render_pass.h"
#include "collection.h"

#include "operations/render.h"

#include "metal/metal_renderer.h"

#include <pagoda/pagoda.h>

#include <pagoda/scene/camera.h>

#include <functional>

namespace pgeditor::renderer
{
const std::string RenderingSystem::GetComponentSystemName() { return "RenderingSystem"; }

void RenderingSystem::Registration(pagoda::Pagoda* pagoda)
{
	using namespace operations;

	auto operationFactory = pagoda->GetOperationFactory();
	auto objectSystem = pagoda->GetProceduralObjectSystem();

	objectSystem->RegisterProceduralComponentSystem(std::make_shared<RenderingSystem>());

	operationFactory->Register(Render::name,
	                           [objectSystem]() { return std::make_shared<operations::Render>(objectSystem); });
}

RenderingSystem::RenderingSystem() : ProceduralComponentSystem(GetComponentSystemName()) {}

void RenderingSystem::DoClone(std::shared_ptr<RenderingComponent> from, std::shared_ptr<RenderingComponent> to)
{
	//
}

pagoda::scene::CameraPtr& RenderingSystem::GetCamera()
{
	if (m_camera == nullptr) {
		m_camera = std::make_shared<pagoda::scene::Camera>();
	}
	return m_camera;
}

void RenderingSystem::Render()
{
  DBG_ASSERT_MSG(m_renderer != nullptr, "Renderer not set");
  if (m_renderer == nullptr) {
    return;
  }

  Collection collection;
  for (const auto& c : m_components) {
    collection.Add(c.second->GetRenderable());
  }
  RenderPass renderPass{collection};

  m_renderer->StartFrame();
  renderPass.Render(m_renderer.get());
  m_renderer->EndFrame();
}

void RenderingSystem::SetRenderer(const std::shared_ptr<Renderer>& renderer) {m_renderer = renderer; }
}  // namespace pgeditor::renderer
