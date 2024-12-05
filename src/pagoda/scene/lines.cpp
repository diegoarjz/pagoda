#include "lines.h"

namespace pagoda::scene {
  namespace {
    std::vector<uint32_t> computeIndices(const std::vector<math::Vec3F>& verts) {
      std::vector<uint32_t> indices;
      indices.reserve(verts.size());
      for (uint32_t i = 0; i < verts.size(); /**/) {
        indices.push_back(i++);
        indices.push_back(i++);
      }
      return indices;
    }
  }

  Lines::Lines(const std::vector<math::Vec3F>& linePoints)
    : Geometry(linePoints, computeIndices(linePoints))
  {
  }
}
