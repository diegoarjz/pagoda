#include "transform.h"

namespace pagoda::scene::nodes {
//----------------------------------------
// \name Ctors and Dtors
Transform::Transform() {}
Transform::~Transform() {}

//----------------------------------------
// \n Transform accessors

const Transformation &Transform::GetTransform() const {
  return m_transform;
}
Transformation &Transform::GetTransform() { return m_transform; }

void Transform::Dirty() { m_dirty = true; }
bool Transform::IsDirty() const { return m_dirty; }

} // namespace pagoda::scene::nodes
