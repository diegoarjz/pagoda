#include "camera_controller.h"

#include <pagoda/math/math_utils.h>

#include <boost/qvm/gen/swizzle3.hpp>
#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_mat_operations.hpp>

#include <QGuiApplication>

using namespace pagoda::math;

namespace pgeditor::gui::viewer {

CameraController::CameraController()
    : m_cameraFocus{0, 0, 0},
      m_horizontalRotation{MathUtils<float>::degrees_to_radians(45)},
      m_verticalRotation{MathUtils<float>::degrees_to_radians(45)}, m_zoom{4} {
  UpdateCamera();
}

void CameraController::MouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() == 0) {
    return;
  }
  auto pos = event->position();
  auto dx = pos.x() - m_lastMousePos.x();
  auto dy = pos.y() - m_lastMousePos.y();

  auto modifiers = QGuiApplication::keyboardModifiers();
  if (modifiers & Qt::KeyboardModifier::AltModifier) {
    auto vec = m_camera.GetRightVector() * dx + m_camera.GetUpVector() * dy;
    if (mag(vec) != 0.0f) {
      m_cameraFocus += boost::qvm::normalized(vec) * 0.1f;
    }
  } else {
    m_horizontalRotation += dx * 0.1;
    m_verticalRotation =
        std::clamp<float>(m_verticalRotation + dy * 0.1f,
                          -0.9f * pagoda::math::MathUtils<float>::half_pi,
                          0.9f * pagoda::math::MathUtils<float>::half_pi);
  }

  m_lastMousePos = event->position();
  UpdateCamera();
}

void CameraController::MousePressEvent(QMouseEvent *event) {
  m_lastMousePos = event->position();
}

void CameraController::MouseReleaseEvent(QMouseEvent *event) {
  m_lastMousePos = event->position();
}

void CameraController::WheelEvent(QWheelEvent *event) {
  m_zoom = std::max<float>(m_zoom + event->angleDelta().y() * 0.1, 0.5f);
  UpdateCamera();
}

void CameraController::UpdateCamera() {
  using namespace boost::qvm;
  using namespace pagoda::math;

  // clang-format off
	auto rotation = translation_mat(m_cameraFocus) *
	                roty_mat<4, float>(m_horizontalRotation) *
                  rotx_mat<4, float>(m_verticalRotation) *
                  translation_mat(Vec3F{0, 0, -m_zoom});
  // clang-format on

  Vec4F newPos = Vec4F(col<3>(rotation));
  auto direction = m_cameraFocus - XYZ(newPos);

  m_camera.SetPosition(XYZ(newPos));
  m_camera.SetViewDirection(direction);
}
} // namespace pgeditor::gui::viewer
