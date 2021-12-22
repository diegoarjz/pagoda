#include "lens.h"

#include <pagoda/math/vec_base.h>

#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>

#include <cmath>

using namespace pagoda::math;
using namespace boost;

namespace pagoda::scene {
Lens::Lens()
    : m_projectionMatrix(qvm::diag_mat(boost::qvm::vec<float, 4>{1, 1, 1, 1})) {
}

void Lens::SetPerspective(float fovY, float aspect, float near, float far) {
  m_projectionMatrix = qvm::perspective_rh(fovY, aspect, near, far);
}

void Lens::SetOrthogonal(float left, float right, float top, float bottom) {
  // m_projectionMatrix = glm::ortho(left, right, top, bottom);
}

const boost::qvm::mat<float, 4, 4> &Lens::GetProjectionMatrix() const {
  return m_projectionMatrix;
}
} // namespace pagoda::scene
