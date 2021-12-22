#pragma once

#include <pagoda/math/vec_base.h>

#include <QMouseEvent>
#include <QPointF>
#include <QWheelEvent>
#include <pagoda/scene/camera.h>

namespace pgeditor::gui::viewer {
class CameraController {
public:
  CameraController();

protected:
  void MouseMoveEvent(QMouseEvent *event);
  void MousePressEvent(QMouseEvent *event);
  void MouseReleaseEvent(QMouseEvent *event);
  void WheelEvent(QWheelEvent *event);

  void UpdateCamera();

  pagoda::math::Vec3F m_cameraFocus{0, 0, 0};
  float m_horizontalRotation{0};
  float m_verticalRotation{0};
  float m_zoom{0};
  QPointF m_lastMousePos{0, 0};

  pagoda::scene::Camera m_camera;
};
} // namespace pgeditor::gui::viewer
