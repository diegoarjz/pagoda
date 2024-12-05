#include "viewer_window.h"

#include "pagoda/scene/camera.h"
#include "pagoda/scene/lines.h"
#include "viewer_camera.h"

#include "pagoda/scene/mesh.h"
#include "pagoda/scene/scene_graph.h"
#include "pagoda/scene/render_target.h"
#include "pgframes/viewer/renderer/render_surface.h"
#include "pgframes/viewer/renderer/gl/frame_buffer.h"
#include "pgframes/viewer/renderer/gl_renderer.h"

#include "pagoda/common/pluggable_factory.h"
#include "pagoda/pagoda.h"

#include "imgui.h"

using namespace pagoda;
using namespace pagoda::objects;
using namespace pagoda::graph;
using namespace pagoda::scene;

namespace pgframes::viewer {

class ViewerWindow::Impl {
public:
  Impl()
    : m_renderer{std::make_shared<renderer::GLRenderer>()}
    , m_sceneGraph{std::make_shared<SceneGraph>()}
    , m_camera{std::make_shared<Camera>()}
    , m_viewerCam{*m_camera}
  {
    // Create the triangle
    std::vector<math::Vec3F> verts{
      {-1, 0, 0},
      {1, 0, 0},
      {0, 1, 0}
    };
    std::vector<uint32_t> indices{0, 1, 2};
    auto mesh = m_sceneGraph->CreateNode<Mesh>(m_sceneGraph->GetRootNode(), Path{"triangle"}, verts, indices);
    std::vector<math::Vec4F> triangleColors{
      {1, 0, 0, 1},
      {0, 1, 0, 1},
      {0, 0, 1, 1}
    };
    mesh->SetVertexColors(triangleColors);

    // Create the axis
    auto axis = m_sceneGraph->CreateNode<Lines>(m_sceneGraph->GetRootNode(), Path{"axis"}, std::vector<math::Vec3F>{
        {0, 0, 0}, {10, 0, 0},
        {0, 0, 0}, {0, 10, 0},
        {0, 0, 0}, {0, 0, 10},
    });
    axis->SetVertexColors(std::vector<math::Vec4F>{
        {1, 0, 0}, {1, 0, 0},
        {0, 1, 0}, {0, 1, 0},
        {0, 0, 1}, {0, 0, 1},
    });

    // Set up viewer camera
    scene::Lens lens;
    lens.SetPerspective(30, 1, 0.01, 1000);
    m_camera->SetLens(lens);
    m_camera->SetPosition({5, 5, 5});
    m_camera->SetTarget({0, 0, 0});
  }

  renderer::RendererPtr m_renderer;
  SceneGraphPtr m_sceneGraph;
  RenderTargetPtr m_renderTarget;
  renderer::RenderSurfacePtr m_renderSurface;
  scene::CameraPtr m_camera;
  ViewerCamera m_viewerCam;
};

ViewerWindow::ViewerWindow() : m_impl{std::make_unique<Impl>()} {
  m_impl->m_renderer->InitRenderer();
  m_impl->m_renderSurface = std::make_shared<renderer::RenderSurface>(800, 600);
  m_impl->m_renderTarget = std::make_shared<scene::RenderTarget>(800, 600, 1);
}

ViewerWindow::~ViewerWindow() {}

const std::string &ViewerWindow::WindowName() {
  static const std::string name{"ViewerWindow"};
  return name;
}

bool ViewerWindow::Draw() {
  static float col[] = {0, 0, 0};
  static float fov = 30;

  /*
  if (ImGui::BeginChild("opts", ImVec2(100, 0))) {
    ImGui::ColorEdit3("bgCol", col);
    ImGui::DragFloat("fov", &fov);

    ImGui::Text("Pan");
    if (ImGui::Button("left")) {
      m_impl->m_viewerCam.Pan(-10, 0);
    }
    ImGui::SameLine();
    if (ImGui::Button("right")) {
      m_impl->m_viewerCam.Pan(10, 0);
    }
    if (ImGui::Button("up")) {
      m_impl->m_viewerCam.Pan(0, 10);
    }
    ImGui::SameLine();
    if (ImGui::Button("down")) {
      m_impl->m_viewerCam.Pan(0, -10);
    }

    ImGui::Text("rotate");
    if (ImGui::Button("orbit left")) {
      m_impl->m_viewerCam.Orbit(-10, 0);
    }
    if (ImGui::Button("orbit right")) {
      m_impl->m_viewerCam.Orbit(10, 0);
    }
    if (ImGui::Button("orbit up")) {
      m_impl->m_viewerCam.Orbit(0, 10);
    }
    if (ImGui::Button("orbit down")) {
      m_impl->m_viewerCam.Orbit(0, -10);
    }

    if (ImGui::Button("reset")) {
      auto& cam = m_impl->m_renderEngine->GetCamera();
      cam.SetPosition({0, 0, 5});
      cam.SetTarget({0, 0, 0});
      m_impl->m_viewerCam.SetPivot({0, 0, 0});
    }

  }
  ImGui::EndChild();

  ImGui::SameLine();
  */

  auto& camera = m_impl->m_camera;

  if (ImGui::BeginChild("RenderArea")) {
    const auto size = ImGui::GetContentRegionAvail();
    m_impl->m_renderSurface->Resize(size.x, size.y);
    m_impl->m_renderTarget->SetWidth(size.x);
    m_impl->m_renderTarget->SetHeight(size.y);

    camera->GetLens().SetPerspective(
        fov, size.x / size.y, 0.01, 1000
    );

    m_impl->m_renderer->SetUpRenderTarget(m_impl->m_renderTarget);
    m_impl->m_renderer->SetViewPort(0, 0, size.x, size.y);
    m_impl->m_renderer->SetClearColor(col[0], col[1], col[2]);

    m_impl->m_renderer->Render(m_impl->m_sceneGraph, camera);

    m_impl->m_renderSurface->Present(m_impl->m_renderTarget);
  }
  ImGui::EndChild();

  // Handle mouse interaction.
  if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
    const auto dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
    ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
    auto& viewerCam = m_impl->m_viewerCam;
    viewerCam.Orbit(dragDelta.x, dragDelta.y);
  }
  if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
    const auto dragDelta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
    auto& viewerCam = m_impl->m_viewerCam;
    viewerCam.Pan(dragDelta.x, -dragDelta.y);
  }
  if (ImGui::IsItemHovered()) {
    auto io = ImGui::GetIO();
    if (io.MouseWheel != 0) {
      m_impl->m_viewerCam.Zoom(io.MouseWheel);
    }
  }

  return true;
}
} // namespace alpha::frontend

extern "C" PAGODA_PLUGIN void *CreateWindow() {
  using FactoryClass =
      pagoda::common::PluggableFactory<pgframes::Window>;
  using BaseRegistrationClass = FactoryClass::Registration;

  struct Reg : public BaseRegistrationClass {
    virtual void Register(FactoryClass *factory) {
      factory->Register("ViewerWindow", []() {
        return std::make_shared<pgframes::viewer::ViewerWindow>();
      });
    }
  };

  return new Reg;
}
