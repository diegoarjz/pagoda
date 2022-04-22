#include "scene_graph.h"

#include <pagoda/common/debug/assertions.h>

#include <entt/entt.hpp>

#include <stack>

namespace pagoda::scene {

class RootSceneNode : public SceneNode {
public:
  ~RootSceneNode() override {}
};

//----------------------------------------
// SceneGraph Pimpl class

struct DisableNotifications {
  DisableNotifications(bool &disable)
      : m_prevDisable{disable}, m_disable{disable} {
    m_disable = false;
  }
  ~DisableNotifications() { m_disable = m_prevDisable; }

private:
  bool m_prevDisable;
  bool &m_disable;
};

class SceneGraph::Impl {
public:
  Impl() : m_rootNode{std::make_shared<RootSceneNode>()} {
    m_rootNode->m_fullPath = Path{"/"};
  }

  void NodeAdded(SceneNodePtr &node) {
    if (m_notificationsEnabled && m_nodeAdded) {
      m_nodeAdded(node);
    }
  }

  void NodeRemoved(SceneNodePtr &node) {
    if (m_notificationsEnabled && m_nodeRemoved) {
      m_nodeRemoved(node);
    }
  }

  void NodeReparented(SceneNodePtr &node, const SceneNodePtr &oldParent,
                      const SceneNodePtr &newParent) {
    if (m_notificationsEnabled && m_nodeReparented) {
      m_nodeReparented(node, oldParent, newParent);
    }
  }

  SceneNodePtr m_rootNode;

  bool m_notificationsEnabled{true};
  entt::delegate<void(SceneNodePtr &)> m_nodeAdded;
  entt::delegate<void(SceneNodePtr &)> m_nodeRemoved;
  entt::delegate<void(SceneNodePtr &, const SceneNodePtr &,
                      const SceneNodePtr &)>
      m_nodeReparented;
};

//----------------------------------------
// SceneGraph class
SceneGraph::SceneGraph() : m_impl{std::make_unique<SceneGraph::Impl>()} {}
SceneGraph::~SceneGraph() {}

///-------------------------------------
/// \name Hierarchy API

SceneNodePtr SceneGraph::GetRootNode() const { return m_impl->m_rootNode; }

void SceneGraph::SetSceneGraphListener(SceneGraphChangeListener *listener) {
  m_impl->m_nodeAdded.connect<&SceneGraphChangeListener::NodeAdded>(*listener);
  m_impl->m_nodeRemoved.connect<&SceneGraphChangeListener::NodeRemoved>(
      listener);
  m_impl->m_nodeReparented.connect<&SceneGraphChangeListener::NodeReparented>(
      listener);
}

void SceneGraph::addNode(SceneNodePtr node, const SceneNodePtr &parent) {
  node->m_sceneGraph = shared_from_this();
  {
    DisableNotifications d(m_impl->m_notificationsEnabled);
    node->SetParent(parent);
  }

  m_impl->NodeAdded(node);
}

bool SceneGraph::reparent(SceneNodePtr node, const SceneNodePtr &oldParent,
                          const SceneNodePtr &newParent) {
  auto prevParent = node->m_parent.expired() ? nullptr : node->m_parent.lock();
  auto nextParent = newParent != nullptr ? newParent : GetRootNode();

  if (prevParent == nextParent) {
    return false;
  }

  if (prevParent != nullptr) {
    prevParent->m_children.erase(node->m_name);
  }

  if (nextParent != nullptr) {
    nextParent->m_children.emplace(node->m_name, node);
  }

  node->m_parent = nextParent;

  if (nextParent != nullptr) {
    node->m_fullPath = nextParent->m_fullPath.AppendChild(node->m_name);
  } else {
    node->m_fullPath = Path("/").AppendChild(node->m_name);
  }

  std::stack<SceneNodePtr> nodes;
  nodes.push(node);
  while (!nodes.empty()) {
    auto n = nodes.top();
    nodes.pop();
    n->m_fullPath = n->GetParent()->m_fullPath.AppendChild(n->m_name);

    for (auto& c : n->m_children) {
      nodes.push(c.second);
    }
  }

  // notify
  m_impl->NodeReparented(node, oldParent, nextParent);

  return true;
}

void SceneGraph::removeNode(SceneNodePtr node) {
  node->m_parent.lock()->m_children.erase(node->m_name);
  node->m_fullPath = Path{};
  node->m_parent.reset();
  node->m_sceneGraph.reset();

  // notify
  m_impl->NodeRemoved(node);
}
} // namespace pagoda::scene
