#pragma once

#include "path.h"
#include "scene_node.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pagoda::scene {

class SceneGraphChangeListener {
public:
  virtual void NodeAdded(SceneNodePtr &node) = 0;
  virtual void NodeRemoved(SceneNodePtr &node) = 0;
  virtual void NodeReparented(SceneNodePtr &node, const SceneNodePtr &oldParent,
                              const SceneNodePtr &newParent) = 0;
};

class SceneGraph : public std::enable_shared_from_this<SceneGraph> {
public:
  ///-------------------------------------
  /// \name Ctors and Dtors

  SceneGraph();
  ~SceneGraph();

  ///-------------------------------------
  /// \name Creation API

  template <class T, typename... Args>
  std::shared_ptr<T> CreateNode(const SceneNodePtr &parent, const Path &name,
                                Args... args) {
    if (parent->GetChild(name) != nullptr) {
      return nullptr;
    }

    std::shared_ptr<T> node = std::make_shared<T>(args...);
    node->m_name = name;
    addNode(node, parent);

    return node;
  }

  template <class T, typename... Args>
  std::shared_ptr<T> CreateNode(const Path &name, Args... args) {
    return CreateNode<T>(GetRootNode(), name, args...);
  }

  ///-------------------------------------
  /// \name Hierarchy API

  SceneNodePtr GetRootNode() const;

  ///-------------------------------------
  /// \name Notifications API

  void SetSceneGraphListener(SceneGraphChangeListener* listener);

private:
  void addNode(SceneNodePtr node, const SceneNodePtr& parent);
  bool reparent(SceneNodePtr node, const SceneNodePtr& oldParent, const SceneNodePtr& newParent);
  void removeNode(SceneNodePtr node);

  class Impl;
  std::unique_ptr<Impl> m_impl;

  friend class SceneNode;
};
} // namespace pagoda::scene
