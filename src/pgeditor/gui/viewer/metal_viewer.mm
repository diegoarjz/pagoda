#include "metal_viewer.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/math/vec_base.h>

#include <pgeditor/renderer/metal/metal_renderer.h>

#include <pgeditor/renderer/buffer.h>
#include <pgeditor/renderer/collection.h>
#include <pgeditor/renderer/renderable.h>
#include <pgeditor/renderer/vertex_attribute.h>

#include <QHBoxLayout>
#include <QWindow>

#import <MetalKit/MetalKit.h>
#include <simd/simd.h>

#include <iostream>
#include <vector>

using namespace pagoda::math;
using namespace pgeditor::renderer;

namespace pgeditor::gui::viewer {
class MetalWindow : public QWindow {
public:
  MetalWindow() {
    setSurfaceType(QSurface::MetalSurface);

    std::vector<Vec4F> translations{{-0.1, 0, 0, 0}, {0.1, 0, 0, 0}};

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
    defaultVert->SetOutput("position", {"position", Type::Vec4});
    network->SetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex,
                                  "defaultVert");

    auto positionNode = network->CreateMaterialNode("bufferView", "position");
    positionNode->SetOutput("position", {"position", Type::Vec4});
    positionNode->SetParameter("bufferName", "position");
    positionNode->SetParameter(
        "semantics", static_cast<int>(VertexAttributeSemantics::Position));
    positionNode->SetParameter("type", static_cast<int>(Type::Vec4));

    defaultVert->ConnectInput("position", positionNode, "position");

    // The frag shader network
    auto defaultFrag =
        network->CreateMaterialNode("defaultFrag", "defaultFrag");
    defaultFrag->SetInput("color", {"color", Type::Vec4});
    defaultFrag->SetInput("scale", {"color", Type::Vec4});
    defaultFrag->SetInput("bias", {"color", Type::Vec4});
    defaultFrag->SetOutput("color", {"color", Type::Vec4});

    auto colorNode = network->CreateMaterialNode("bufferView", "color");
    colorNode->SetOutput("color", {"color", Type::Vec4});
    colorNode->SetParameter("bufferName", "color");
    colorNode->SetParameter("semantics",
                            static_cast<int>(VertexAttributeSemantics::Color));
    colorNode->SetParameter("type", static_cast<int>(Type::Vec4));

    auto scaleUniformNode = network->CreateMaterialNode("uniformView", "scale");
    scaleUniformNode->SetOutput("scale", {"scale", Type::Vec4});
    scaleUniformNode->SetParameter("uniformName", "scale");
    scaleUniformNode->SetParameter("type", static_cast<int>(Type::Vec4));

    auto biasUniformNode = network->CreateMaterialNode("uniformView", "bias");
    biasUniformNode->SetOutput("bias", {"bias", Type::Vec4});
    biasUniformNode->SetParameter("uniformName", "bias");
    biasUniformNode->SetParameter("type", static_cast<int>(Type::Vec4));

    defaultFrag->ConnectInput("color", colorNode, "color");
    defaultFrag->ConnectInput("scale", scaleUniformNode, "scale");
    defaultFrag->ConnectInput("bias", biasUniformNode, "bias");

    network->SetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment,
                                  "defaultFrag");

    for (const auto &t : translations) {
      auto renderable = std::make_shared<Renderable>();

      auto pointsCopy = position;
      auto colorsCopy = colors;
      for (auto &p : pointsCopy) {
        p += t;
      }
      renderable->GetBuffer("position")->SetData(pointsCopy);
      renderable->GetBuffer("color")->SetData(colorsCopy);
      renderable->SetMaterial(network);

      m_renderCollection.Add(renderable);
    }
  }

  ~MetalWindow() override {}

  void exposeEvent(QExposeEvent *e) override {
    initMetal();
    m_metalRenderer->Draw(m_renderCollection);
    requestUpdate();
  }

  void updateEvent() { m_metalRenderer->Draw(m_renderCollection); }

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
