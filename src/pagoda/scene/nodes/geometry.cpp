#include "geometry.h"

namespace pagoda::scene::nodes {
//----------------------------------------
// \name Ctors and Dtors
Geometry::Geometry() : SceneNode() {}
Geometry::~Geometry() {}

//----------------------------------------
// \name Geometry accessors
geometry::core::GeometryPtr Geometry::GetGeometry() const { return m_geometry; }

void Geometry::SetGeometry(const geometry::core::GeometryPtr &geo) { m_geometry = geo; }
} // namespace pagoda::scene::nodes
