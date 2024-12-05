#include "mesh.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/exception/unimplemented.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/scene/camera.h>

#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>

using namespace pagoda::math;
using namespace boost::qvm;

#define GL_CHECK(cmd) \
cmd; \
{ \
	auto error = glGetError(); \
	if (error != GL_NO_ERROR) { \
    LOG_ERROR(#cmd) \
    LOG_FATAL(" error: " << error); \
	} \
}

namespace pagoda::renderer::gl
{

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices,
           PrimitiveType primType)
  : m_vertices(vertices)
  , m_indices(indices)
  , m_primitiveType{primType}
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

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  GL_CHECK(glGenVertexArrays(1, &m_vertexArrayObject));
  GL_CHECK(glGenBuffers(1, &m_vertexBufferObject));
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  GL_CHECK(glBindVertexArray(m_vertexArrayObject));

  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW));

  GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
  GL_CHECK(glEnableVertexAttribArray(0));

  GL_CHECK(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float))));
  GL_CHECK(glEnableVertexAttribArray(1));

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0););

  GL_CHECK(glGenBuffers(1, &m_elementBufferId));
  GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferId));
  GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSizeInBytes(), GetIndexData(), GL_STATIC_DRAW));

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  GL_CHECK(glBindVertexArray(0));
}

void Mesh::Render()
{
  GLenum primType = GL_TRIANGLES;
  switch (m_primitiveType) {
    case PrimitiveType::Triangles:  primType = GL_TRIANGLES;    break;
    case PrimitiveType::Lines:      primType = GL_LINES;        break;
    case PrimitiveType::LineStrip:  primType = GL_LINE_STRIP;   break;
    case PrimitiveType::LineLoop:   primType = GL_LINE_LOOP;    break;
  };
  GL_CHECK(glBindVertexArray(m_vertexArrayObject));
  // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	GL_CHECK(glDrawElements(primType, IndexCount(), GL_UNSIGNED_INT, (void *)0));
}

void Mesh::Dispose() { 
  /*
  UNIMPLEMENTED;
  */
}

}  // namespace pgeditor::renderer::gl
