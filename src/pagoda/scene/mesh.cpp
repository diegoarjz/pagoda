#include "mesh.h"

namespace pagoda::scene {

Mesh::Mesh(const std::vector<math::Vec3F>& verts, const std::vector<uint32_t>& indices)
  : Geometry(verts, indices)
  {
  }
}
