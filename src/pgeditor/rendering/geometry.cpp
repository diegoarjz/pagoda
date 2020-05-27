#include "geometry.h"

#include "gl_debug.h"

#include <pgeditor/rendering/vertex.h>

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/unimplemented.h>
#include <pagoda/common/instrument/profiler.h>

#include <GL/glew.h>

namespace pgeditor::rendering
{
Geometry::Geometry(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    : m_vertices(vertices), m_indices(indices)
{
}
Geometry::~Geometry() {}

std::size_t Geometry::VertexCount() const { return m_vertices.size(); }
std::size_t Geometry::VertexBufferSizeInBytes() const { return VertexCount() * sizeof(Vertex); }
const Vertex *Geometry::GetVertexData() const { return &(m_vertices[0]); }

std::size_t Geometry::IndexCount() const { return m_indices.size(); }
std::size_t Geometry::IndexBufferSizeInBytes() const { return IndexCount() * sizeof(uint32_t); }
const uint32_t *Geometry::GetIndexData() const { return &(m_indices[0]); }

void Geometry::DoLoad(Renderer *r)
{
	START_PROFILE;

	LOG_DEBUG("Loading Geometry");

	// Create vertex array object
	CHECK_GL_ERROR(glGenVertexArrays(1, &m_vertexArrayObject));
	CHECK_GL_ERROR(glBindVertexArray(m_vertexArrayObject));

	// Create vertex buffer
	CHECK_GL_ERROR(glGenBuffers(1, &m_vertexBufferObject));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject));
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER, VertexBufferSizeInBytes(), GetVertexData(), GL_STATIC_DRAW));

	// Position Attrib Array
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));

	// Normal Attrib Array
	CHECK_GL_ERROR(glEnableVertexAttribArray(1));
	CHECK_GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
	                                     reinterpret_cast<void *>(3 * sizeof(float))));

	// Texture coordinates
	CHECK_GL_ERROR(glEnableVertexAttribArray(2));
	CHECK_GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
	                                     reinterpret_cast<void *>(6 * sizeof(float))));

	// Create Element Buffer
	CHECK_GL_ERROR(glGenBuffers(1, &m_elementBufferId));
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSizeInBytes(), GetIndexData(), GL_STATIC_DRAW));
}

void Geometry::DoRender(Renderer *r)
{
	START_PROFILE;

	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject));
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId));
	CHECK_GL_ERROR(glBindVertexArray(m_vertexArrayObject));

	CHECK_GL_ERROR(glEnableVertexAttribArray(0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(1));
	CHECK_GL_ERROR(glEnableVertexAttribArray(2));

	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, IndexCount(), GL_UNSIGNED_INT, (void *)0));

	CHECK_GL_ERROR(glDisableVertexAttribArray(0));
	CHECK_GL_ERROR(glDisableVertexAttribArray(1));
	CHECK_GL_ERROR(glDisableVertexAttribArray(2));
}

void Geometry::DoDispose(Renderer *r)
{
	//
	UNIMPLEMENTED;
}

}  // namespace pgeditor::rendering
