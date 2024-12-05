#pragma once

#include "pagoda/math/vec_base.h"

namespace pagoda::scene {
  class Camera;
}

namespace pgframes::viewer {
class ViewerCamera {
  public:
    ViewerCamera(pagoda::scene::Camera& cam);

    void Orbit(float horizontal, float vertical);

    void Zoom(float amount);

    void Pan(float horizontal, float vertical);

    void SetPivot(const pagoda::math::Vec3F& pivot);
  private:
    pagoda::math::Vec3F m_pivotPoint{0, 0, 0};
    pagoda::scene::Camera& m_camera;
};
}
