#include "mesh.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/unimplemented.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/scene/camera.h>

#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>

using namespace pagoda::math;
using namespace boost::qvm;

namespace pgeditor::renderer::gl
{
Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
  : m_vertices(vertices), m_indices(indices)
{
}
Mesh::~Mesh() {}

std::size_t Mesh::VertexCount() const { return m_vertices.size(); }
std::size_t Mesh::VertexBufferSizeInBytes() const { return VertexCount() * sizeof(Vertex); }
const Mesh::Vertex *Mesh::GetVertexData() const { return &(m_vertices[0]); }

std::size_t Mesh::IndexCount() const { return m_indices.size(); }
std::size_t Mesh::IndexBufferSizeInBytes() const { return IndexCount() * sizeof(uint32_t); }
const uint32_t *Mesh::GetIndexData() const { return &(m_indices[0]); }

void Mesh::Load()
{
	START_PROFILE;

	LOG_INFO("Loading mesh");

	// Create vertex array object
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	// Create vertex buffer
	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSizeInBytes(), GetVertexData(), GL_STATIC_DRAW);

	// Position Attrib Array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Normal Attrib Array
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*
	// Texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
	*/

	// Create Element Buffer
	glGenBuffers(1, &m_elementBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSizeInBytes(), GetIndexData(), GL_STATIC_DRAW);

	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG_FATAL("Unable to create mesh. GL Error: " << error);
	}

	LOG_INFO("Mesh VAO: " << m_vertexArrayObject);
	LOG_INFO("Mesh VBO: " << m_vertexBufferObject);
	LOG_INFO("Mesh ELE: " << m_elementBufferId);
}

void Mesh::Render()
{
	START_PROFILE;

	// glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId);
	glBindVertexArray(m_vertexArrayObject);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	// glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, IndexCount(), GL_UNSIGNED_INT, (void *)0);
	auto error = glGetError();
	if (error != GL_NO_ERROR) {
		LOG_FATAL("Unable to render mesh. GL Error: " << error);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	// glDisableVertexAttribArray(2);
}

void Mesh::Dispose() { UNIMPLEMENTED; }

}  // namespace pgeditor::renderer::gl
