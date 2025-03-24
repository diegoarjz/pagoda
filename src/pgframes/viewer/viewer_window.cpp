#include "viewer_window.h"

#include "pagoda/scene/camera.h"
#include "pagoda/scene/lines.h"
#include "pgframes/widgets/label.h"
#include "pgframes/widgets/tree_view.h"
#include "viewer_camera.h"

#include "pagoda/scene/mesh.h"
#include "pagoda/scene/scene_graph.h"
#include "pagoda/scene/render_target.h"
#include "pgframes/viewer/renderer/render_surface.h"
#include "pgframes/viewer/renderer/gl/frame_buffer.h"
#include "pgframes/viewer/renderer/gl_renderer.h"

#include "pgframes/widgets/color_edit.h"
#include "pgframes/widgets/float_edit.h"

#include "pagoda/common/pluggable_factory.h"
#include "pagoda/pagoda.h"

#include "imgui.h"
#include <boost/qvm/gen/swizzle3.hpp>

using namespace pagoda;
using namespace pagoda::objects;
using namespace pagoda::graph;
using namespace pagoda::scene;

namespace pgframes::viewer {

class SceneGraphTreeViewDelegate : public widgets::TreeView::Delegate {
public:
  SceneGraphTreeViewDelegate(const SceneGraphPtr& scenegraph)
    : m_sceneGraph{scenegraph}
  {
  }

  void Roots(widgets::TreeView::NodeHandleVisitor f) {
    f(m_sceneGraph->GetRootNode());
  }

  std::string Label(const widgets::TreeView::NodeHandle& node) {
    return std::any_cast<SceneNodePtr>(node)->GetFullPath().ToString();
  }

  void Children(const widgets::TreeView::NodeHandle& n, widgets::TreeView::NodeHandleVisitor f) {
    SceneNodePtr node = std::any_cast<SceneNodePtr>(n);
    node->ForEachChild([f](auto sceneNode) {
      f(sceneNode);
      return true;
    });
  }

  uint32_t NumChildren(const widgets::TreeView::NodeHandle& node) const {
    return std::any_cast<SceneNodePtr>(node)->GetChildCount();
  }

  SceneGraphPtr m_sceneGraph;
};

class ViewerWindow::Impl {
public:
  Impl()
    : m_renderer{std::make_shared<renderer::GLRenderer>()}
    , m_sceneGraph{std::make_shared<SceneGraph>()}
    , m_camera{std::make_shared<Camera>()}
    , m_viewerCam{*m_camera}
    , m_sceneGraphTreeViewDelegate{std::make_shared<SceneGraphTreeViewDelegate>(m_sceneGraph)}
  {
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

  // TODO: Clean this
  SceneNodePtr m_selectedSceneNode{nullptr};

  void drawTreeNode(SceneNodePtr node) {
    std::string label = node->GetFullPath().ToString();

    if (node->GetChildCount() == 0) {
      if (ImGui::Selectable( label.c_str())) {
        m_selectedSceneNode = node;
        m_selectedNodeLabel->SetText(m_selectedSceneNode->GetFullPath().ToString());
      }
    }
    else {
      if (ImGui::TreeNodeEx(label.c_str())) {
        if (ImGui::IsItemClicked()) {
          m_selectedSceneNode = node;
          m_selectedNodeLabel->SetText(m_selectedSceneNode->GetFullPath().ToString());
        }
        node->ForEachChild([this](const SceneNodePtr& c) {
          this->drawTreeNode(c);
          return true;
        });
        ImGui::TreePop();
      }
    }
  }

  std::shared_ptr<widgets::VerticalLayout> m_sidebarLayout;
  std::shared_ptr<widgets::ColorEdit> m_bgColorEdit;
  std::shared_ptr<widgets::FloatEdit> m_fovEdit;

  // selected node
  std::shared_ptr<widgets::Label> m_selectedNodeLabel;
  std::shared_ptr<widgets::Float3Edit> m_selecteNodePos;
  std::shared_ptr<widgets::Float3Edit> m_selecteNodeRot;
  std::shared_ptr<widgets::Float3Edit> m_selecteNodeScale;

  // Scenegraph
  std::shared_ptr<SceneGraphTreeViewDelegate> m_sceneGraphTreeViewDelegate;
  std::shared_ptr<widgets::TreeView> m_sceneGraphTree;

  math::Vec3F m_bgColor{0, 0, 0};
  float m_fov{30.0f};
};

ViewerWindow::ViewerWindow() : m_impl{std::make_unique<Impl>()} {
  m_impl->m_renderer->InitRenderer();
  m_impl->m_renderSurface = std::make_shared<renderer::RenderSurface>(800, 600);
  m_impl->m_renderTarget = std::make_shared<scene::RenderTarget>(800, 600, 1);

  // sidebar
  m_impl->m_sidebarLayout = std::make_shared<widgets::VerticalLayout>();
  m_impl->m_bgColorEdit = std::make_shared<widgets::ColorEdit>(math::Vec3F{0, 0, 0});
  m_impl->m_fovEdit = std::make_shared<widgets::FloatEdit>(m_impl->m_fov);

  m_impl->m_selectedNodeLabel = std::make_shared<widgets::Label>("selected node");
  m_impl->m_selecteNodePos = std::make_shared<widgets::Float3Edit>(math::Vec3F{0, 0, 0});
  m_impl->m_selecteNodeRot = std::make_shared<widgets::Float3Edit>(math::Vec3F{0, 0, 0});
  m_impl->m_selecteNodeScale = std::make_shared<widgets::Float3Edit>(math::Vec3F{0, 0, 0});
  m_impl->m_sceneGraphTree = std::make_shared<widgets::TreeView>(m_impl->m_sceneGraphTreeViewDelegate);

  m_impl->m_sidebarLayout->AddWdiget(m_impl->m_bgColorEdit);
  m_impl->m_sidebarLayout->AddWdiget(m_impl->m_fovEdit);

  m_impl->m_sidebarLayout->AddWdiget(m_impl->m_selectedNodeLabel);
  m_impl->m_sidebarLayout->AddWdiget(m_impl->m_selecteNodePos);
  m_impl->m_sidebarLayout->AddWdiget(m_impl->m_selecteNodeRot);
  m_impl->m_sidebarLayout->AddWdiget(m_impl->m_selecteNodeScale);

  m_impl->m_sidebarLayout->AddWdiget(m_impl->m_sceneGraphTree);

  m_impl->m_bgColorEdit->OnColorChanged([this](const auto& col) {
    m_impl->m_bgColor = boost::qvm::XYZ(col);
  });
  m_impl->m_fovEdit->OnValueChanged([this](const auto& val) { m_impl->m_fov = val; });
  m_impl->m_selecteNodePos->OnValueChanged([this](const auto& val) {
    if (m_impl->m_selectedSceneNode != nullptr) {
      m_impl->m_selectedSceneNode->SetPosition(val);
    }
  });
  m_impl->m_selecteNodeRot->OnValueChanged([this](const auto& val) {
    if (m_impl->m_selectedSceneNode != nullptr) {
      m_impl->m_selectedSceneNode->SetRotation(val);
    }
  });
  m_impl->m_selecteNodeScale->OnValueChanged([this](const auto& val) {
    if (m_impl->m_selectedSceneNode != nullptr) {
      m_impl->m_selectedSceneNode->SetScale(val);
    }
  });

  m_impl->m_sceneGraphTree->OnPressed([this](const auto& val) {
    m_impl->m_selectedSceneNode = std::any_cast<SceneNodePtr>(val);
    m_impl->m_selectedNodeLabel->SetText(m_impl->m_selectedSceneNode->GetFullPath().ToString());
  });
}

ViewerWindow::~ViewerWindow() {}

const std::string &ViewerWindow::WindowName() {
  static const std::string name{"ViewerWindow"};
  return name;
}


bool ViewerWindow::Draw() {
  if (ImGui::BeginChild("opts", ImVec2(200, 0))) {
    m_impl->m_sidebarLayout->Draw();

    //m_impl->drawTreeNode(m_impl->m_sceneGraph->GetRootNode());

    /*
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
      auto& cam = m_impl->m_camera;
      cam->SetPosition({0, 0, 5});
      cam->SetTarget({0, 0, 0});
      m_impl->m_viewerCam.SetPivot({0, 0, 0});
    }
    */

  }
  ImGui::EndChild();

  ImGui::SameLine();

  auto& camera = m_impl->m_camera;

  if (ImGui::BeginChild("RenderArea")) {
    const auto size = ImGui::GetContentRegionAvail();
    m_impl->m_renderSurface->Resize(size.x, size.y);
    m_impl->m_renderTarget->SetWidth(size.x);
    m_impl->m_renderTarget->SetHeight(size.y);

    camera->GetLens().SetPerspective(
        m_impl->m_fov, size.x / size.y, 0.01, 1000
    );

    m_impl->m_renderer->SetUpRenderTarget(m_impl->m_renderTarget);
    m_impl->m_renderer->SetViewPort(0, 0, size.x, size.y);
    m_impl->m_renderer->SetClearColor(X(m_impl->m_bgColor), Y(m_impl->m_bgColor), Z(m_impl->m_bgColor));

    m_impl->m_renderer->Render(m_impl->m_sceneGraph, camera);

    m_impl->m_renderSurface->Present(m_impl->m_renderTarget);
  }

  // Handle mouse interaction.
  if (ImGui::IsItemHovered()) {
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
    auto io = ImGui::GetIO();
    if (io.MouseWheel != 0) {
      m_impl->m_viewerCam.Zoom(io.MouseWheel);
    }
  }

  ImGui::EndChild();

  return true;
}

pagoda::scene::SceneGraphPtr ViewerWindow::GetSceneGraph() const {
  return m_impl->m_sceneGraph;
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
