#pragma once

#include "pagoda/geometry/core/geometry.h"
#include "pagoda/scene/scene_node.h"

namespace pagoda::scene::nodes {
class Geometry : public SceneNode {
public:
  //----------------------------------------
  // \name Ctors and Dtors
  Geometry();
  ~Geometry() override;

  //----------------------------------------
  // \name Geometry accessors
  geometry::core::GeometryPtr GetGeometry() const;
  void SetGeometry(const geometry::core::GeometryPtr& geo);

private:
  geometry::core::GeometryPtr m_geometry{nullptr};
};
} // namespace pagoda::scene::nodes
