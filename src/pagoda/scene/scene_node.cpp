#include "scene_node.h"

#include "scene_graph.h"

#include <pagoda/common/debug/assertions.h>

namespace pagoda::scene {

SceneNode::SceneNode() {}
SceneNode::~SceneNode() {}

///-------------------------------------
/// \name Getters & Setterss
const Path &SceneNode::GetName() const { return m_name; }
const Path &SceneNode::GetFullPath() const { return m_fullPath; }
SceneGraphPtr SceneNode::GetSceneGraph() const { return m_sceneGraph.lock(); }

///-------------------------------------
/// \name Hierarchy API
SceneNodePtr SceneNode::GetParent() const { return m_parent.lock(); }

void SceneNode::ForEachChild(std::function<bool(const SceneNodePtr &)> f) {
  for (auto c : m_children) {
    f(c.second);
  }
}

SceneNodePtr SceneNode::GetChild(const Path &name) {
  auto iter = m_children.find(name);
  if (iter == m_children.end()) {
    return nullptr;
  }
  return iter->second;
}

std::size_t SceneNode::GetChildCount() const { return m_children.size(); }

bool SceneNode::SetParent(const SceneNodePtr &newParent) {
  auto This = shared_from_this();
  return GetSceneGraph()->reparent(This, GetParent(), newParent);
}

void SceneNode::Remove() {
  auto This = shared_from_this();
  GetSceneGraph()->removeNode(This);
}

} // namespace pagoda::scene
