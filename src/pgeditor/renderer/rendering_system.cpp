#include "rendering_system.h"

#include "gl/mesh.h"
#include "operations/render.h"

#include <pagoda/scene/camera.h>
#include <pagoda/objects/operation_factory.h>
#include <pagoda/objects/procedural_object_system.h>

#include <pagoda/pagoda.h>

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if (m_renderPass == nullptr) {
		m_renderPass = std::make_shared<gl::RenderPass>();
		m_renderPass->Load();
	}

	if (m_gridTask == nullptr) {
		m_gridTask = std::make_shared<gl::GridRenderTask>();
		m_gridTask->Load();
	}

	m_gridTask->SetProjectionMatrix(m_camera->GetProjectionMatrix());
	m_gridTask->SetViewMatrix(m_camera->GetViewMatrix());
	m_gridTask->Run();

	m_renderPass->SetProjectionMatrix(m_camera->GetProjectionMatrix());
	m_renderPass->SetViewMatrix(m_camera->GetViewMatrix());
	m_renderPass->SetUp();

	std::unordered_set<RenderingComponentPtr> toRender;
	std::unordered_set<RenderingComponentPtr> toLoad;
	std::unordered_set<RenderingComponentPtr> toDispose;

	std::vector<RenderingComponentPtr> components;
	for (const auto& c : m_components) {
		components.push_back(c.second);
		toRender.insert(c.second);
		if (m_lastRendered.find(c.second) == m_lastRendered.end()) {
			toLoad.insert(c.second);
		}
	}

	for (const auto& c : m_lastRendered) {
		if (toRender.find(c) == toRender.end()) {
			toDispose.insert(c);
		}
	}
	for (const auto& c : toLoad) {
		c->GetMesh()->Load();
	}
	for (const auto& c : toDispose) {
		c->GetMesh()->Dispose();
	}

	m_renderPass->Render(components);

	m_renderPass->TearDown();

	m_lastRendered.swap(toRender);
}
}  // namespace pgeditor::renderer
