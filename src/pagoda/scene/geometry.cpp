#include "geometry.h"

namespace pagoda::scene
{
Geometry::Geometry(const std::vector<math::Vec3F>& verts,
                   const std::vector<uint32_t>& indices)
  : m_vertices{verts}
  , m_indices{indices}
{
}

const std::vector<math::Vec3F>& Geometry::GetVertices() const { return m_vertices; }
const std::vector<uint32_t>& Geometry::GetIndices() const { return m_indices; }

void Geometry::SetVertexColors(const std::vector<math::Vec4F>& colors) { m_vertexColors = colors; }
const std::vector<math::Vec4F>& Geometry::GetVertexColors() {
  if (m_vertexColors.empty()) {
    m_vertexColors.resize(m_vertices.size());
    std::fill(m_vertexColors.begin(), m_vertexColors.end(), math::Vec4F{0.18, 0.18, 0.18, 1.0});
  }
  return m_vertexColors;
}
}
