#pragma once

#include "pagoda/math/vec_base.h"
#include "pagoda/scene/geometry.h"

#include <vector>

namespace pagoda::scene {
class Lines : public Geometry {
public:
  Lines(const std::vector<math::Vec3F>& linePoints);

  PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Lines; }
};
}
