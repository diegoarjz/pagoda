#pragma once

#include <GL/glew.h>

#include <pgeditor/renderer/gl/default_material.h>
#include <pgeditor/renderer/gl/mesh.h>

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <pagoda/scene/camera.h>
#include <pagoda/scene/lens.h>

#include <QMouseEvent>
#include <QtOpenGLWidgets/QOpenGLWidget>

#include <memory>

namespace pgeditor::renderer
{
using RenderingSystemPtr = std::shared_ptr<class RenderingSystem>;
using RenderingComponentPtr = std::shared_ptr<class RenderingComponent>;
}  // namespace pgeditor::renderer

namespace pgeditor::gui::viewer
{
class Viewer : public QOpenGLWidget
{
	public:
	Viewer(QWidget* parent);
	~Viewer() override;

	void SetRenderingSystem(const renderer::RenderingSystemPtr& renderingSystem);

	protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	private:
	void updateCamera();

	renderer::RenderingSystemPtr m_renderingSystem;

	// TODO: MOVE this out of here
	pagoda::math::Vec3F m_cameraFocus;
	float m_horizontalRotation;
	float m_verticalRotation;
	float m_zoom;
	QPointF m_lastMousePos;
};
}  // namespace pgeditor::gui::viewer
