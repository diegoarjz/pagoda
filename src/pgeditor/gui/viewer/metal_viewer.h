#pragma once

#include <QWidget>

namespace pgeditor::renderer
{
using RenderingSystemPtr = std::shared_ptr<class RenderingSystem>;
}  // namespace pgeditor::renderer


namespace pgeditor::gui::viewer {
class MetalViewer : public QWidget {
public:
  MetalViewer(QWidget *parent);
  ~MetalViewer() override;

  void SetRenderingSystem(const renderer::RenderingSystemPtr& renderingSystem);

private:
  QWindow* m_metalWindow;
};
} // namespace pgeditor::gui::viewer
