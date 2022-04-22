#pragma once

#include "path.h"

#include <pagoda/common/visitor.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace pagoda::scene {
//----------------------------------------
// Forward declarations
using SceneGraphWeakPtr = std::weak_ptr<class SceneGraph>;
using SceneGraphPtr = std::shared_ptr<class SceneGraph>;
using SceneNodeWeakPtr = std::weak_ptr<class SceneNode>;
using SceneNodePtr = std::shared_ptr<class SceneNode>;

using SceneNodeVector = std::vector<SceneNodePtr>;
using SceneNodeMap = std::unordered_map<Path, SceneNodePtr>;

class SceneNode : public std::enable_shared_from_this<SceneNode>,
                  public common::Visitable<SceneNode> {
public:
  ///-------------------------------------
  /// \name Ctors and Dtors

  SceneNode();
  virtual ~SceneNode();

  ///-------------------------------------
  /// \name Getters & Setterss
  const Path &GetName() const;
  const Path &GetFullPath() const;

  ///-------------------------------------
  /// \name Hierarchy API
  SceneNodePtr GetParent() const;
  SceneGraphPtr GetSceneGraph() const;

  void ForEachChild(std::function<bool(const SceneNodePtr &)> f);
  SceneNodePtr GetChild(const Path &name);
  std::size_t GetChildCount() const;

  bool SetParent(const SceneNodePtr &newParent);
  void Remove();

private:
  SceneGraphWeakPtr m_sceneGraph;
  Path m_name;
  Path m_fullPath;

  SceneNodeWeakPtr m_parent;
  SceneNodeMap m_children;

  friend class SceneGraph;
};
} // namespace pagoda::scene
