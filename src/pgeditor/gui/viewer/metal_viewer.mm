#include "metal_viewer.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/math/vec_base.h>

#include <pgeditor/renderer/metal/metal_renderer.h>

#include <pgeditor/renderer/collection.h>
#include <pgeditor/renderer/renderable.h>
#include <pgeditor/renderer/buffer.h>

#include <QHBoxLayout>
#include <QWindow>

#import <MetalKit/MetalKit.h>
#include <simd/simd.h>

#include <iostream>
#include <vector>

using namespace pagoda::math;
using namespace pgeditor::renderer;

namespace pgeditor::gui::viewer
{
class MetalWindow : public QWindow
{
	public:
	MetalWindow()
  {
    setSurfaceType(QSurface::MetalSurface);

#if 1
    std::vector<Vec4F> translations{
      {-100, 0, 0, 0},
      { 100, 0, 0, 0}
    };

    static const std::vector<Vec4F> position{
      {  255, -255, 0, 1},
      { -255, -255, 0, 1},
      {    0,  255, 0, 1}
    };
#else
    std::vector<Vec4F> translations{
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    };

    static const std::vector<Vec4F> position{
      {  1, -1, 0, 1},
      { -1, -1, 0, 1},
      {  0,  1, 0, 1}
    };
#endif
    static const std::vector<Vec4F> colors{
      { 1, 0, 0, 1},
      { 0, 1, 0, 1},
      { 0, 0, 1, 1},
    };

    auto network = std::make_shared<MaterialNetwork>("default");
    auto node = network->CreateMaterialNode("default", "defaultFrag");
    node->SetOutput("color", {"color", Type::Vec4});
    network->SetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment, "defaultFrag");

    for (const auto& t : translations) {
      auto renderable = std::make_shared<Renderable>();

      auto pointsCopy = position;
      auto colorsCopy = colors;
      for (auto& p : pointsCopy) {
        p += t;
      }
      renderable->GetBuffer("position").SetData(pointsCopy);
      renderable->GetBuffer("color").SetData(colorsCopy);
      renderable->SetMaterial(network);

      m_renderCollection.Add(renderable);
    }
  }

	~MetalWindow() override {}

	void exposeEvent(QExposeEvent* e) override
	{
		initMetal();
		m_metalRenderer->Draw(m_renderCollection);
		requestUpdate();
	}

	void updateEvent() { m_metalRenderer->Draw(m_renderCollection); }

	bool event(QEvent* e) override
	{
		if (e->type() == QEvent::UpdateRequest) {
			updateEvent();
			return false;
		}
		return QWindow::event(e);
	}

	void initMetal()
	{
		if (m_metalRenderer != nullptr) {
			return;
		}

		CAMetalLayer* metalLayer = reinterpret_cast<CAMetalLayer*>(reinterpret_cast<NSView*>(winId()).layer);
		m_metalRenderer = std::make_shared<renderer::metal::MetalRenderer>(metalLayer);
		m_metalRenderer->InitRenderer();
	}

	private:
	std::shared_ptr<renderer::metal::MetalRenderer> m_metalRenderer;

  renderer::Collection m_renderCollection;
};

MetalViewer::MetalViewer(QWidget* parent) : QWidget(parent)
{
	QHBoxLayout* hLayout = new QHBoxLayout();
	this->setLayout(hLayout);

	auto window = new MetalWindow;
	QWidget* widget = QWidget::createWindowContainer(window);
	widget->setContentsMargins(0, 0, 0, 0);
	hLayout->addWidget(widget);
}

MetalViewer::~MetalViewer() {}
}  // namespace pgeditor::gui::viewer
