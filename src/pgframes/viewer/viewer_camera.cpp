#include "viewer_camera.h"

#include "pagoda/scene/camera.h"
#include "pagoda/scene/transformation.h"

#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>
#include <boost/qvm/swizzle.hpp>

using namespace pagoda;

namespace pgframes::viewer {
  ViewerCamera::ViewerCamera(scene::Camera& cam)
      : m_camera{cam}
  {
  }

  void ViewerCamera::Orbit(float horizontal, float vertical) {
    math::Vec4F position = XYZ1(m_camera.GetPosition());
    const math::Vec4F pivot = XYZ1(m_pivotPoint);
    float xAngle = horizontal * 0.01f;
    float yAngle = vertical * 0.01f;

    // Extra step to handle the problem when the camera direction is the same as the up vector
    float cosAngle = boost::qvm::dot(m_camera.GetViewDirection(),
                                     pagoda::scene::Transformation::upVector);
    if (cosAngle > 0.99f) {
      yAngle = 0.0f;
    }

    // step 2: Rotate the camera around the pivot point on the first axis.
    math::Mat4x4F rotationMatrixX = boost::qvm::identity_mat<float, 4>();
    rotationMatrixX = boost::qvm::rot_mat<4>(pagoda::scene::Transformation::upVector, xAngle);
    position = (rotationMatrixX * (position - pivot)) + pivot;

    // step 3: Rotate the camera around the pivot point on the second axis.
    math::Mat4x4F rotationMatrixY = boost::qvm::identity_mat<float, 4>();
    rotationMatrixY = boost::qvm::rot_mat<4>(m_camera.GetRightVector(), yAngle);
    math::Vec4F finalPosition = (rotationMatrixY * (position - pivot)) + pivot;

    // Update the camera view (we keep the same lookat and the same up vector)
    m_camera.SetCameraView(XYZ(finalPosition), m_pivotPoint);
  }

  void ViewerCamera::Zoom(float amount) {
    const math::Vec3F newPos = m_camera.GetPosition() + 
      (m_pivotPoint - m_camera.GetPosition()) * amount * 0.01f;
    m_camera.SetPosition(newPos);
  }

  void ViewerCamera::Pan(float horizontal, float vertical) {
    const auto position = m_camera.GetPosition();
    const auto cameraUp = m_camera.GetUpVector() * vertical * 0.01f;
    const auto cameraRight = m_camera.GetRightVector() * horizontal * 0.01f;
    const math::Vec3F translation = cameraUp + cameraRight;
    m_pivotPoint += translation;
    m_camera.SetPosition(position + translation);
    m_camera.SetTarget(m_pivotPoint);
  }

  void ViewerCamera::SetPivot(const math::Vec3F& pivot) { m_pivotPoint = pivot; }
}
