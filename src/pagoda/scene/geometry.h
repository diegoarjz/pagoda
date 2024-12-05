#pragma once

#include "pagoda/math/vec_base.h"
#include "pagoda/scene/scene_node.h"

namespace pagoda::scene
{
class Geometry : public SceneNode {
public:
  enum class PrimitiveType {
    Triangles,
    Lines,
    LineStrip,
    LineLoop
  };

  Geometry(const std::vector<math::Vec3F>& verts,
           const std::vector<uint32_t>& indices);
  ~Geometry() override = default;

  const std::vector<math::Vec3F>& GetVertices() const;
  const std::vector<uint32_t>& GetIndices() const;

  void SetVertexColors(const std::vector<math::Vec4F>& colors);
  const std::vector<math::Vec4F>& GetVertexColors();

  virtual PrimitiveType GetPrimitiveType() const = 0;

protected:
  std::vector<math::Vec3F> m_vertices;
  std::vector<uint32_t> m_indices;
  std::vector<math::Vec4F> m_vertexColors;
};
}
