#pragma once

#include "lens.h"

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

namespace pagoda::scene
{
//! Represents a Camera in the 3D scene.
class Camera
{
public:
  //! Constructs a camera in the origin of coordinates, looking towards
  //! the negative z-axis.
  Camera();

  //! Sets the camera position.
  void SetPosition(const math::Vec3F &pos);
  //! Sets to where the camera is looking
  void SetTarget(const math::Vec3F &target);
  //! Returns the camera target.
  const math::Vec3F& GetTarget() const;

  //! Returns the camera position.
  math::Vec3F GetPosition() const;
  //! Returns the camera view direction.
  math::Vec3F GetViewDirection() const;
  //! Returns the camera right direction.
  math::Vec3F GetRightVector() const;
  //! Returns the camera up direction.
  math::Vec3F GetUpVector() const;

  void SetCameraView(const math::Vec3F& eye, const math::Vec3F& lookat);

  //! Returns the view matrix.
  math::Mat4x4F GetViewMatrix();
  //! Returns the projection matrix which is provided by the Lens.
  const math::Mat4x4F &GetProjectionMatrix();

  //! Sets the camera Lens.
  void SetLens(const Lens &lens);
  //! Returns the camera Lens.
  Lens &GetLens();

private:
  math::Vec3F m_position;       //< The camera position.
  math::Vec3F m_target;         //< Where the camera is looking at.
  math::Mat4x4F m_viewMatrix;   //< The view matrix.
	Lens m_lens;                  //< The camera Lens.
};

using CameraPtr = std::shared_ptr<Camera>;
}  // namespace pagoda::scene
