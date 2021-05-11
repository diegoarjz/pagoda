#pragma once

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <GL/glew.h>
#include <vector>

namespace pgeditor::renderer::gl
{
class Mesh
{
	public:
	struct Vertex
	{
		pagoda::math::Vec3F m_position;
		pagoda::math::Vec3F m_normal;
		pagoda::math::Vec2F m_texCoords;
	};

	Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
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
	void createShaderProgram();

	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;

	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_elementBufferId;
	GLuint m_shaderProgramId;

	GLuint m_projectionMatrixUniform;
	GLuint m_viewMatrixUniform;
	GLuint m_modelMatrixUniform;
};
}  // namespace pgeditor::renderer::gl
