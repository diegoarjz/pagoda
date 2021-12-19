#pragma once

#include "buffer.h"
#include "material_network.h"

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
  const std::shared_ptr<MaterialNetwork> &GetMaterial() const {
    return m_material;
  }
  void SetMaterial(const std::shared_ptr<MaterialNetwork> &material) {
    m_material = material;
  }

private:
  PrimitiveType m_primitiveType;

  std::unordered_map<std::string, Buffer> m_buffers;
  std::shared_ptr<MaterialNetwork> m_material;
};

using RenderablePtr = std::shared_ptr<Renderable>;
} // namespace pgeditor::renderer
