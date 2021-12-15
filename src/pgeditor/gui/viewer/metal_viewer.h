#pragma once

#include <QWidget>

namespace pgeditor::gui::viewer
{
class MetalViewer : public QWidget
{
	public:
	MetalViewer(QWidget* parent);
	~MetalViewer() override;

	protected:
	private:
};
}  // namespace pgeditor::gui::viewer
