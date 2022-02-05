#include "renderable.h"

#include <boost/qvm/mat_operations.hpp>

namespace pgeditor::renderer {
Renderable::Renderable() : m_primitiveType{PrimitiveType::Triangle} {
  m_modelMatrix = boost::qvm::identity_mat<float, 4>();
}

Buffer *Renderable::GetBuffer(const std::string &name) {
  auto iter = m_buffers.find(name);
  if (iter == m_buffers.end()) {
    return &m_buffers.emplace(name, Buffer()).first->second;
  }
  return &iter->second;
}

Buffer *Renderable::GetIndexBuffer() { return &m_indexBuffer; }

std::size_t Renderable::GetVertexCount() {
  return GetBuffer("position")->GetNumElements();
}

const Renderable::PrimitiveType &Renderable::GetPrimitiveType() const {
  return m_primitiveType;
}

const std::shared_ptr<pagoda::material::MaterialNetwork> &Renderable::GetMaterial() const {
  return m_material;
}

void Renderable::SetMaterial(const std::shared_ptr<pagoda::material::MaterialNetwork> &material) {
  m_material = material;
}

void Renderable::SetModelMatrix(const pagoda::math::Mat4x4F &m) {
  m_modelMatrix = m;
}
const pagoda::math::Mat4x4F Renderable::GetModelMatrix() const {
  return m_modelMatrix;
}

} // namespace pgeditor::renderer
