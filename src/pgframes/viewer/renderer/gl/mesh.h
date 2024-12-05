#pragma once

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <GL/glew.h>

#include <vector>
#include <memory>

namespace pagoda::renderer::gl
{
class Mesh
{
	public:
  enum class PrimitiveType {
    Triangles,
    Lines,
    LineStrip,
    LineLoop
  };

	struct Vertex
	{
		pagoda::math::Vec3F position;
    pagoda::math::Vec4F color;
		//pagoda::math::Vec3F m_normal;
		//pagoda::math::Vec2F m_texCoords;
	};

	Mesh(const std::vector<Vertex>& vertices,
       const std::vector<uint32_t>& indices,
       PrimitiveType primType = PrimitiveType::Triangles);
	~Mesh();

	std::size_t VertexCount() const;
	std::size_t VertexBufferSizeInBytes() const;
	const Vertex* GetVertexData() const;

	std::size_t IndexCount() const;
	std::size_t IndexBufferSizeInBytes() const;
	const uint32_t* GetIndexData() const;

	void Load();
	void Render();
	void Dispose();

	private:

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

  PrimitiveType m_primitiveType;
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferId;
};

using MeshPtr = std::shared_ptr<Mesh>;
}  // namespace pagoda::renderer::gl
