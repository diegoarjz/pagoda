#pragma once

#include <pagoda/math/matrix_base.h>

#include <GL/glew.h>

namespace pgeditor::renderer::gl
{
class GridRenderTask
{
	public:
	GridRenderTask();

	void Load();
	void Run();
	void Dispose();

	void SetProjectionMatrix(const pagoda::math::Mat4x4F &mat);
	void SetViewMatrix(const pagoda::math::Mat4x4F &mat);

	private:
	GLuint m_gridVAO;
	GLuint m_gridVBO;
	GLuint m_gridElements;
	GLuint m_gridShaderId;
	GLuint m_projectionMatrixUniform;
	GLuint m_viewMatrixUniform;

	pagoda::math::Mat4x4F m_projectionMatrix;
	pagoda::math::Mat4x4F m_viewMatrix;
};
}  // namespace pgeditor::renderer::gl
