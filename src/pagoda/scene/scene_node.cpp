#include "scene_node.h"

#include "scene_graph.h"

#include <pagoda/common/debug/assertions.h>

#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/swizzle.hpp>

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

void SceneNode::ForEachChild(std::function<bool(SceneNodePtr )> f) {
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

///----------------------------------------
/// \name Transform API
void SceneNode::SetPosition(const math::Vec3F& pos)
{
  m_position = pos;
  m_localMatrixDirty = true;
  dirtyWorldMatrix();
}

void SceneNode::SetScale(const math::Vec3F& scale) 
{
  m_scale = scale;
  m_localMatrixDirty = true;
  dirtyWorldMatrix();
}

void SceneNode::SetRotation(const math::Vec3F& rot, RotOrder rotOrder)
{
  m_rotation = rot;
  m_rotOrder = rotOrder;
  m_localMatrixDirty = true;
  dirtyWorldMatrix();
}

math::Mat4x4F SceneNode::GetLocalMatrix()
{
  if (m_localMatrixDirty) {
    const math::Mat4x4F scaleMat = boost::qvm::diag_mat(XYZ1(m_scale));
    const auto rotMatrix = [this] () -> auto {
      switch (m_rotOrder) {
        case RotOrder::XYZ: return boost::qvm::rot_mat_xyz<4>(m_rotation.a[0], m_rotation.a[1], m_rotation.a[2]);
        case RotOrder::XZY: return boost::qvm::rot_mat_xzy<4>(m_rotation.a[0], m_rotation.a[1], m_rotation.a[2]);
        case RotOrder::YXZ: return boost::qvm::rot_mat_yxz<4>(m_rotation.a[0], m_rotation.a[1], m_rotation.a[2]);
        case RotOrder::YZX: return boost::qvm::rot_mat_yzx<4>(m_rotation.a[0], m_rotation.a[1], m_rotation.a[2]);
        case RotOrder::ZXY: return boost::qvm::rot_mat_zxy<4>(m_rotation.a[0], m_rotation.a[1], m_rotation.a[2]);
        case RotOrder::ZYX: 
        default:            return boost::qvm::rot_mat_zyx<4>(m_rotation.a[0], m_rotation.a[1], m_rotation.a[2]);
      };
    };
    const math::Mat4x4F translateMat = boost::qvm::translation_mat(m_position);
    m_localMatrix = translateMat * rotMatrix() * scaleMat;
    m_localMatrixDirty = false;
  }
  return m_localMatrix;
}

math::Mat4x4F SceneNode::GetWorldMatrix()
{
  if (m_worldMatrixDirty || m_localMatrixDirty) {
    if (auto parent = GetParent()) {
      m_worldMatrix = parent->GetWorldMatrix() * GetLocalMatrix();
    }
    else {
      m_worldMatrix = GetLocalMatrix();
    }
    m_worldMatrixDirty = false;
  }
  return m_worldMatrix;
}

void SceneNode::dirtyWorldMatrix() {
  m_worldMatrixDirty = true;
  for (const auto& c : m_children) {
    c.second->dirtyWorldMatrix();
  }
}

} // namespace pagoda::scene
