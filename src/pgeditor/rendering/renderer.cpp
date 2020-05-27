#include "renderer.h"

#include "bound_state.h"
#include "gl_debug.h"
#include "material.h"
#include "pagoda/image/image.h"
#include "renderable_object.h"
#include "shader.h"
#include "shader_program.h"

#include <pgeditor/scene/attachable.h>
#include <pgeditor/scene/attachable_visitor.h>
#include <pgeditor/scene/camera.h>
#include <pgeditor/scene/mesh.h>
#include <pgeditor/scene/node.h>
#include <memory>

#include <pgeditor/rendering/geometry.h>

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <GL/glew.h>

#include <boost/qvm/map_mat_mat.hpp>
#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>

#include <queue>
#include <unordered_map>

using namespace pagoda::math;
using namespace pgeditor::scene;
using namespace boost;

namespace pgeditor::rendering
{
class Renderer::Impl
{
public:
	Impl(Renderer* r) : m_renderer(r) {}

	class RendererAttachableVisitor final : public pgeditor::scene::IAttachableVisitor
	{
	public:
		RendererAttachableVisitor() {}

		void Visit(scene::Camera* camera) override
		{
			START_PROFILE;
			camera->SetTransformation(m_node->GetWorldTransformation());
		}

		void Visit(scene::Mesh* mesh) override
		{
			START_PROFILE;

			auto renderableObject = mesh->GetRenderableObject();
			if (renderableObject == nullptr)
			{
				renderableObject = std::make_shared<RenderableObject>(mesh->GetGeometry(), mesh->GetMaterial());
				mesh->SetRenderableObject(renderableObject);
			}

			renderableObject->SetModelMatrix(m_node->GetWorldTransformation().GetTransformationMatrix());

			m_renderableObjects.push_back(renderableObject);
		}

		void SetNode(std::shared_ptr<Node> n) { m_node = n; }

		std::vector<std::shared_ptr<RenderableObject>>&& GetRenderableObjects()
		{
			return std::move(m_renderableObjects);
		}

	private:
		std::shared_ptr<scene::Node> m_node;

		std::vector<std::shared_ptr<RenderableObject>> m_renderableObjects;
	};

	void SetCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }

	void RenderScene(std::shared_ptr<Node> sceneRootNode)
	{
		START_PROFILE;

		std::queue<std::shared_ptr<Node>> nodes;
		nodes.push(sceneRootNode);

		RendererAttachableVisitor v;

		while (!nodes.empty())
		{
			auto n = nodes.front();
			nodes.pop();

			v.SetNode(n);
			for (auto a : n->GetAttachables())
			{
				a->AcceptVisitor(&v);
			}

			for (auto c : n->GetChildren())
			{
				nodes.push(c);
			}
		}

		renderObjects(std::move(v.GetRenderableObjects()));
	}

private:
	void renderObjects(std::vector<std::shared_ptr<RenderableObject>>&& objects)
	{
		for (auto& o : objects)
		{
			o->SetViewMatrix(m_camera->GetViewMatrix());
			o->SetProjectionMatrix(m_camera->GetProjectionMatrix());
			o->Render(m_renderer);
		}
	}

	std::shared_ptr<Camera> m_camera;
	Renderer* m_renderer;
};  // namespace selector

Renderer::Renderer() : m_implementation(std::make_unique<Impl>(this)) {}
Renderer::~Renderer() {}

void Renderer::SetCamera(std::shared_ptr<Camera> camera) { m_implementation->SetCamera(camera); }

void Renderer::RenderScene(std::shared_ptr<Node> sceneRootNode) { m_implementation->RenderScene(sceneRootNode); }

}  // namespace pgeditor::rendering
