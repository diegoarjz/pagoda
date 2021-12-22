#include "metal_viewer.h"

#include "camera_controller.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/math/vec_base.h>

#include <pagoda/scene/camera.h>
#include <pagoda/scene/transformation.h>

#include <pgeditor/renderer/metal/metal_renderer.h>

#include <pgeditor/renderer/buffer.h>
#include <pgeditor/renderer/collection.h>
#include <pgeditor/renderer/renderable.h>
#include <pgeditor/renderer/vertex_attribute.h>

#include <QHBoxLayout>
#include <QResizeEvent>
#include <QWindow>

#import <MetalKit/MetalKit.h>
#include <simd/simd.h>

#include <iostream>
#include <vector>

using namespace pagoda::math;
using namespace pagoda::scene;
using namespace pgeditor::renderer;

namespace pgeditor::gui::viewer {
class MetalWindow : public QWindow, public CameraController {
public:
  MetalWindow() {
    setSurfaceType(QSurface::MetalSurface);

    Lens l;
    l.SetPerspective(MathUtils<float>::degrees_to_radians(30), 1, 0.001, 100);
    std::vector<Vec3F> translations{{0.0, 0, -3.5}, {0.1, 0, 0}};

    static const std::vector<Vec4F> position{
        {-1, -1, 0, 1}, {1, -1, 0, 1}, {0, 1, 0, 1}};
    static const std::vector<Vec4F> colors{
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 1, 1},
    };

    // Create a material network
    auto network = std::make_shared<MaterialNetwork>("default");
    // The Vert shader network
    auto defaultVert =
        network->CreateMaterialNode("defaultVert", "defaultVert");
    defaultVert->SetInput("position", {"position", Type::Vec4});
    defaultVert->SetInput("projectionMatrix", {"position", Type::Mat4});
    defaultVert->SetInput("viewMatrix", {"position", Type::Mat4});
    defaultVert->SetInput("modelMatrix", {"position", Type::Mat4});
    defaultVert->SetOutput("position", {"position", Type::Vec4});
    network->SetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex,
                                  "defaultVert");

    auto positionNode = network->CreateMaterialNode("bufferView", "position");
    positionNode->SetOutput("position", {"position", Type::Vec4});
    positionNode->SetParameter("bufferName", "position");
    positionNode->SetParameter(
        "semantics", static_cast<int>(VertexAttributeSemantics::Position));
    positionNode->SetParameter("type", static_cast<int>(Type::Vec4));

    auto projMatrix =
        network->CreateMaterialNode("uniformView", "projectionMatrix");
    projMatrix->SetOutput("projectionMatrix", {"projectionMatrix", Type::Mat4});
    projMatrix->SetParameter("uniformName", "projectionMatrix");
    projMatrix->SetParameter("type", static_cast<int>(Type::Mat4));

    auto viewMatrix = network->CreateMaterialNode("uniformView", "viewMatrix");
    viewMatrix->SetOutput("viewMatrix", {"viewMatrix", Type::Mat4});
    viewMatrix->SetParameter("uniformName", "viewMatrix");
    viewMatrix->SetParameter("type", static_cast<int>(Type::Mat4));

    auto modelMatrix =
        network->CreateMaterialNode("uniformView", "modelMatrix");
    modelMatrix->SetOutput("modelMatrix", {"modelMatrix", Type::Mat4});
    modelMatrix->SetParameter("uniformName", "modelMatrix");
    modelMatrix->SetParameter("type", static_cast<int>(Type::Mat4));

    defaultVert->ConnectInput("position", positionNode, "position");
    defaultVert->ConnectInput("projectionMatrix", projMatrix,
                              "projectionMatrix");
    defaultVert->ConnectInput("viewMatrix", viewMatrix, "viewMatrix");
    defaultVert->ConnectInput("modelMatrix", modelMatrix, "modelMatrix");

    // The frag shader network
    auto defaultFrag =
        network->CreateMaterialNode("defaultFrag", "defaultFrag");
    defaultFrag->SetInput("color", {"color", Type::Vec4});
    defaultFrag->SetOutput("color", {"color", Type::Vec4});

    auto colorNode = network->CreateMaterialNode("bufferView", "color");
    colorNode->SetOutput("color", {"color", Type::Vec4});
    colorNode->SetParameter("bufferName", "color");
    colorNode->SetParameter("semantics",
                            static_cast<int>(VertexAttributeSemantics::Color));
    colorNode->SetParameter("type", static_cast<int>(Type::Vec4));

    defaultFrag->ConnectInput("color", colorNode, "color");

    network->SetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment,
                                  "defaultFrag");

    for (const auto &t : translations) {
      auto renderable = std::make_shared<Renderable>();
      Transformation transform;
      transform.Translate(t);

      auto pointsCopy = position;
      auto colorsCopy = colors;
      renderable->GetBuffer("position")->SetData(pointsCopy);
      renderable->GetBuffer("color")->SetData(colorsCopy);
      renderable->SetMaterial(network);

      renderable->SetModelMatrix(transform.GetTransformationMatrix());

      m_renderCollection.Add(renderable);
    }
  }

  ~MetalWindow() override {}

  void exposeEvent(QExposeEvent *e) override {
    initMetal();
    m_metalRenderer->SetCamera(m_camera);
    m_metalRenderer->Draw(m_renderCollection);
    requestUpdate();
  }

  void updateEvent() {
    m_metalRenderer->SetCamera(m_camera);
    m_metalRenderer->Draw(m_renderCollection);
  }

  void resizeEvent(QResizeEvent *e) override {
    QWindow::resizeEvent(e);
    const float width = e->size().width();
    const float height = e->size().height();
    const auto pixelRatio = QWindow::devicePixelRatio();
    Lens l;
    l.SetPerspective(MathUtils<float>::degrees_to_radians(30), width / height,
                     0.001, 100);
    m_camera.SetLens(l);

    if (m_metalRenderer != nullptr) {
      m_metalRenderer->SetDisplaySize(
          {static_cast<uint32_t>(e->size().width() * pixelRatio),
           static_cast<uint32_t>(e->size().height() * pixelRatio)});
      m_metalRenderer->SetCamera(m_camera);
      m_metalRenderer->Draw(m_renderCollection);
    }
  }

  bool event(QEvent *e) override {
    if (e->type() == QEvent::UpdateRequest) {
      updateEvent();
      return false;
    }
    return QWindow::event(e);
  }

  void initMetal() {
    if (m_metalRenderer != nullptr) {
      return;
    }

    CAMetalLayer *metalLayer = reinterpret_cast<CAMetalLayer *>(
        reinterpret_cast<NSView *>(winId()).layer);
    m_metalRenderer =
        std::make_shared<renderer::metal::MetalRenderer>(metalLayer);
    m_metalRenderer->InitRenderer();
  }

  void mouseMoveEvent(QMouseEvent *event) override {
    MouseMoveEvent(event);
    updateEvent();
  }
  void mousePressEvent(QMouseEvent *event) override {
    MousePressEvent(event);
    QWindow::mousePressEvent(event);
  }
  void mouseReleaseEvent(QMouseEvent *event) override {
    MouseReleaseEvent(event);
    QWindow::mouseReleaseEvent(event);
  }

  void wheelEvent(QWheelEvent *event) override {
    WheelEvent(event);
    QWindow::wheelEvent(event);
    updateEvent();
  }

private:
  std::shared_ptr<renderer::metal::MetalRenderer> m_metalRenderer;

  renderer::Collection m_renderCollection;
};

MetalViewer::MetalViewer(QWidget *parent) : QWidget(parent) {
  QHBoxLayout *hLayout = new QHBoxLayout();
  this->setLayout(hLayout);

  auto window = new MetalWindow;
  QWidget *widget = QWidget::createWindowContainer(window);
  widget->setContentsMargins(0, 0, 0, 0);
  hLayout->addWidget(widget);
}

MetalViewer::~MetalViewer() {}
} // namespace pgeditor::gui::viewer
