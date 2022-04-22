#pragma once

#include "pagoda/scene/scene_node.h"
#include "pagoda/scene/transformation.h"

namespace pagoda::scene::nodes {
class Transform : public SceneNode {
public:
  //----------------------------------------
  // \name Ctors and Dtors
  Transform();
  ~Transform() override;

  //----------------------------------------
  // \name Transform accessors
  const Transformation& GetTransform() const;
  Transformation& GetTransform();

  void Dirty();
  bool IsDirty() const;

private:
  Transformation m_transform{};
  bool m_dirty{true};
};
} // namespace pagoda::scene::nodes
