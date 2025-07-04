#include "gl_renderer.h"
#include "pagoda/common/debug/logger.h"
#include "pagoda/scene/camera.h"
#include "pagoda/scene/lines.h"
#include "pagoda/scene/mesh.h"
#include "pagoda/scene/scene_graph.h"
#include "pagoda/scene/scene_node.h"
#include "pagoda/scene/render_target.h"
#include "pgframes/viewer/renderer/gl/frame_buffer.h"
#include "pgframes/viewer/renderer/renderable.h"

#include <GL/glew.h>
#ifdef PAGODA_OS_MACOS
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <boost/qvm/swizzle.hpp>

#include <iostream>

namespace pagoda::renderer {
using scene::SceneNodePtr;

class GLRenderer::Impl {
public:
  ~Impl() {}

  bool m_initialized{false};

  scene::RenderTargetPtr m_renderTarget;
  gl::FrameBufferPtr m_frameBuffer;

  std::unordered_map<scene::Path, RenderablePtr> m_renderables;
};

GLRenderer::GLRenderer()
  : m_impl{std::make_unique<Impl>()}
{
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::InitRenderer()
{
  if (m_impl->m_initialized) {
    return;
  }
  m_impl->m_initialized = true;

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cerr << "Unable to initialize GLEW from gl_renderer.cpp" << std::endl;
  }

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
}

void GLRenderer::Render(const scene::SceneGraphPtr& scene, const scene::CameraPtr& camera)
{
  if (scene == nullptr) {
    LOG_WARNING("No scene given to render engine.");
    return;
  }

  SceneNodePtr root = scene->GetNodeAtPath(scene::Path{"/"});
  if (root == nullptr) {
    LOG_WARNING("Trying to render a scene without a root node.");
    return;
  }

  auto& cachedRenderables = m_impl->m_renderables;

  // Collect renderables
  std::stack<scene::SceneNodePtr> nodes;
  nodes.push(root);
  while (!nodes.empty()) {
    auto node = nodes.top();
    nodes.pop();

    const auto& nodePath = node->GetFullPath();
    if (auto geometryNode = std::dynamic_pointer_cast<scene::Geometry>(node)) {
      if (cachedRenderables.find(nodePath) == cachedRenderables.end()) {
        const auto& verts = geometryNode->GetVertices();
        const auto& indices = geometryNode->GetIndices();
        const auto& colors = geometryNode->GetVertexColors();
        std::vector<gl::Mesh::Vertex> vertices(verts.size());

        for (uint32_t i = 0; i < verts.size(); ++i) {
          auto& vert = vertices[i];
          vert.position = verts[i];
          vert.color = colors[i];
        }

        gl::Mesh::PrimitiveType primType;
        switch (geometryNode->GetPrimitiveType()) {
          case scene::Geometry::PrimitiveType::Triangles:  primType = gl::Mesh::PrimitiveType::Triangles;   break;
          case scene::Geometry::PrimitiveType::Lines:      primType = gl::Mesh::PrimitiveType::Lines;       break;
          case scene::Geometry::PrimitiveType::LineStrip:  primType = gl::Mesh::PrimitiveType::LineStrip;   break;
          case scene::Geometry::PrimitiveType::LineLoop:   primType = gl::Mesh::PrimitiveType::LineLoop;    break;
        };

        auto mesh = std::make_shared<gl::Mesh>(vertices, indices, primType);
        mesh->Load();
        auto shader = std::make_shared<gl::ShaderProgram>();
        shader->Load();

        auto renderable = std::make_shared<Renderable>(mesh, shader);

        cachedRenderables.emplace(nodePath, renderable);
      }
    }

    node->ForEachChild([&nodes] (const auto& n) {
      nodes.push(n);
      return true;
    });
  }

  // Start rendering
  StartFrame();

  for (auto& renderable : cachedRenderables) {
    auto mesh = renderable.second->GetMesh();
    auto shader = renderable.second->GetShader();

    auto node = scene->GetNodeAtPath(renderable.first);
    renderable.second->SetWorldMatrix(node->GetWorldMatrix());

    shader->Use();
    shader->SetModelMatrix(renderable.second->GetWorldMatrix());
    shader->SetViewMatrix(camera->GetViewMatrix());
    shader->SetProjectionMatrix(camera->GetProjectionMatrix());

    mesh->Render();
  }

  EndFrame();
}

void GLRenderer::StartFrame()
{
  const auto renderTargetDims = m_impl->m_renderTarget->GetDimensions();
  const auto& frameBuffer = m_impl->m_frameBuffer;
  if (XY(renderTargetDims) != m_impl->m_frameBuffer->GetDimensions()) {
    frameBuffer->RescaleFrameBuffer(X(renderTargetDims), Y(renderTargetDims));
  }

  m_impl->m_frameBuffer->Bind();
  glViewport(X(m_viewPort), Y(m_viewPort), Z(m_viewPort), W(m_viewPort));
  glClearColor(X(m_clearColor), Y(m_clearColor), Z(m_clearColor), 1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::EndFrame()
{
  m_impl->m_frameBuffer->Unbind();
}

void GLRenderer::StartRenderPass()
{
}

void GLRenderer::EndRenderPass()
{
}

void GLRenderer::SetUpRenderTarget(scene::RenderTargetPtr renderTarget)
{
  if (m_impl->m_renderTarget != renderTarget) {
    m_impl->m_frameBuffer = std::make_shared<gl::FrameBuffer>(renderTarget->GetWidth(),
                                                              renderTarget->GetHeight());
    m_impl->m_renderTarget = renderTarget;
    m_impl->m_renderTarget->SetHandle(m_impl->m_frameBuffer);
  }
}
} // namespace pagoda::renderer
