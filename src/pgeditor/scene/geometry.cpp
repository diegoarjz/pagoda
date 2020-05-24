#include "geometry.h"

#include <pgeditor/rendering/vertex.h>

using namespace pgeditor::rendering;

namespace pgeditor::scene
{
Geometry::Geometry(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    : m_vertices(vertices), m_indices(indices), m_bindableId(0), m_bound(BoundState::Unbound)
{
}

std::size_t Geometry::VertexCount() const { return m_vertices.size(); }
std::size_t Geometry::VertexBufferSizeInBytes() const { return VertexCount() * sizeof(Vertex); }
const Vertex *Geometry::GetVertexData() const { return &(m_vertices[0]); }

std::size_t Geometry::IndexCount() const { return m_indices.size(); }
std::size_t Geometry::IndexBufferSizeInBytes() const { return IndexCount() * sizeof(uint32_t); }
const uint32_t *Geometry::GetIndexData() const { return &(m_indices[0]); }

std::size_t Geometry::GetBindableId() const { return m_bindableId; }

BoundState Geometry::GetBoundState() const { return m_bound; }
void Geometry::Bind(const std::size_t &id)
{
	m_bindableId = id;
	m_bound = BoundState::Bound;
}

void Geometry::Unbind() { m_bound = BoundState::Unbound; }
}  // namespace pgeditor::scene
