#pragma once

#include <pgeditor/rendering/bound_state.h>

#include <vector>

namespace pgeditor::rendering
{
struct Vertex;
}

namespace pgeditor::scene
{
class Geometry
{
public:
	Geometry(const std::vector<rendering::Vertex>& vertices, const std::vector<uint32_t>& indices);

	std::size_t VertexCount() const;
	std::size_t VertexBufferSizeInBytes() const;
	const rendering::Vertex* GetVertexData() const;

	std::size_t IndexCount() const;
	std::size_t IndexBufferSizeInBytes() const;
	const uint32_t* GetIndexData() const;

	std::size_t GetBindableId() const;

	rendering::BoundState GetBoundState() const;
	void Bind(const std::size_t& id);
	void Unbind();

private:
	/// Vertex Buffer
	std::vector<rendering::Vertex> m_vertices;
	/// Index Buffer
	std::vector<uint32_t> m_indices;
	/// Bindable Id
	std::size_t m_bindableId;
	/// Bound state
	rendering::BoundState m_bound;
};
}  // namespace pgeditor::scene
