#pragma once

#include "bound_state.h"
#include "renderable.h"

#include <GL/glew.h>
#include <vector>

namespace pgeditor::rendering
{
struct Vertex;

class Geometry : public Renderable
{
public:
	Geometry(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	virtual ~Geometry();

	std::size_t VertexCount() const;
	std::size_t VertexBufferSizeInBytes() const;
	const rendering::Vertex* GetVertexData() const;

	std::size_t IndexCount() const;
	std::size_t IndexBufferSizeInBytes() const;
	const uint32_t* GetIndexData() const;

public:
	void DoLoad(Renderer* r) override;
	void DoRender(Renderer* r) override;
	void DoDispose(Renderer* r) override;

private:
	/// Vertex Buffer
	std::vector<rendering::Vertex> m_vertices;
	/// Index Buffer
	std::vector<uint32_t> m_indices;

	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferId;
};
}  // namespace pgeditor::rendering
