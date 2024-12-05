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
#include <GL/gl.h>

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

  // Collect renderables
  std::vector<std::shared_ptr<Renderable>> renderables;

  std::stack<scene::SceneNodePtr> nodes;
  nodes.push(root);
  while (!nodes.empty()) {
    auto node = nodes.top();
    nodes.pop();

    if (auto geometryNode = std::dynamic_pointer_cast<scene::Geometry>(node)) {
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
      auto shader = std::make_shared<gl::ShaderProgram>();

      auto renderable = std::make_shared<Renderable>(mesh, shader);
      renderable->SetWorldMatrix({1.0});

      renderables.push_back(renderable);
    }

    node->ForEachChild([&nodes] (const auto& n) {
      nodes.push(n);
      return true;
    });
  }

  // Start rendering
  StartFrame();

  for (auto& renderable : renderables) {
    auto mesh = renderable->GetMesh();
    auto shader = renderable->GetShader();

    mesh->Load();
    shader->Load();

    shader->Use();
    shader->SetModelMatrix({1.0});
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
