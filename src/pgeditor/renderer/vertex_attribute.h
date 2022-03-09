#pragma once

#include <pagoda/common/types.h>

namespace pgeditor::renderer {
enum class VertexAttributeSemantics {
  Position,
  Normal,
  TexCoord,
  Color,
  Custom,
  Invalid
};

struct VertexAttributeDescription {
  std::string name;
  pagoda::common::Type m_type;
  std::size_t m_componentsPerElement;
  std::size_t m_bytesPerComponent;
};

} // namespace pgeditor::renderer
