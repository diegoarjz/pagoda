#pragma once

#include <pagoda/material/material_network.h>

#include "buffer.h"

#include <pagoda/math/vec_base.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace pgeditor::renderer {
class Renderable {
public:
  enum class PrimitiveType { Triangle };

  Renderable();

  const PrimitiveType &GetPrimitiveType() const;
  std::size_t GetVertexCount();

  Buffer *GetBuffer(const std::string &name);
  Buffer *GetIndexBuffer();

  const std::shared_ptr<pagoda::material::MaterialNetwork> &GetMaterial() const;
  void SetMaterial(const std::shared_ptr<pagoda::material::MaterialNetwork> &material);
  void SetModelMatrix(const pagoda::math::Mat4x4F &m);
  const pagoda::math::Mat4x4F GetModelMatrix() const;

private:
  PrimitiveType m_primitiveType;

  std::unordered_map<std::string, Buffer> m_buffers;
  Buffer m_indexBuffer;
  std::shared_ptr<pagoda::material::MaterialNetwork> m_material;
  pagoda::math::Mat4x4F m_modelMatrix;
};

using RenderablePtr = std::shared_ptr<Renderable>;
} // namespace pgeditor::renderer
