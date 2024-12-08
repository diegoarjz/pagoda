#pragma once

#include "path.h"

#include "pagoda/math/matrix_base.h"
#include "pagoda/math/vec_base.h"

#include <pagoda/common/visitor.h>

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

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

  void ForEachChild(std::function<bool(SceneNodePtr )> f);
  SceneNodePtr GetChild(const Path &name);
  std::size_t GetChildCount() const;

  bool SetParent(const SceneNodePtr &newParent);
  void Remove();

  ///----------------------------------------
  /// \name Transform API
  enum class RotOrder { XYZ, XZY, YXZ, YZX, ZXY, ZYX };

  void SetPosition(const math::Vec3F& pos);
  math::Vec3F GetPosition() const { return m_position; };
  void SetScale(const math::Vec3F& scale);
  math::Vec3F GetScale() const { return m_scale; };
  void SetRotation(const math::Vec3F& rot, RotOrder rotOrder = RotOrder::XYZ);
  math::Vec3F GetRotation() const { return m_rotation; };

  math::Mat4x4F GetLocalMatrix();
  math::Mat4x4F GetWorldMatrix();

private:

  void dirtyWorldMatrix();

  SceneGraphWeakPtr m_sceneGraph;
  Path m_name;
  Path m_fullPath;

  SceneNodeWeakPtr m_parent;
  SceneNodeMap m_children;

  math::Vec3F m_position{0, 0, 0};
  math::Vec3F m_scale{1, 1, 1};
  math::Vec3F m_rotation{0, 0, 0};
  RotOrder m_rotOrder{RotOrder::XYZ};
  bool m_localMatrixDirty{true};
  math::Mat4x4F m_localMatrix;
  bool m_worldMatrixDirty{true};
  math::Mat4x4F m_worldMatrix;

  friend class SceneGraph;
};
} // namespace pagoda::scene
