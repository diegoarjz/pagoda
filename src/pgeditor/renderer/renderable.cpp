#include "renderable.h"

namespace pgeditor::renderer
{
Renderable::Renderable() : m_primitiveType{PrimitiveType::Triangle} {}

Buffer& Renderable::GetBuffer(const std::string& name) { return m_buffers[name]; }

std::size_t Renderable::GetVertexCount() { return GetBuffer("position").GetNumElements(); }

const Renderable::PrimitiveType& Renderable::GetPrimitiveType() const { return m_primitiveType; }
}  // namespace pgeditor::renderer
