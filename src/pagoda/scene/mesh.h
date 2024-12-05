#pragma once

#include "pagoda/math/vec_base.h"
#include "pagoda/scene/geometry.h"

namespace pagoda::scene {
class Mesh : public Geometry {
public:
  Mesh(const std::vector<math::Vec3F>& verts, const std::vector<uint32_t>& indices);

  PrimitiveType GetPrimitiveType() const override { return PrimitiveType::Triangles; }
};
}
