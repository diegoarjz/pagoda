#pragma once

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

#include <GL/glew.h>

namespace pgeditor::renderer::gl
{
class DefaultMaterial
{
	public:
	void Load();
	void Render();
	void Dispose();

	void SetProjectionMatrix(const pagoda::math::Mat4x4F& mat);
	void SetViewMatrix(const pagoda::math::Mat4x4F& mat);
	void SetModelMatrix(const pagoda::math::Mat4x4F& mat);

	private:
	GLuint m_shaderProgramId;

	GLuint m_projectionMatrixUniform;
	GLuint m_viewMatrixUniform;
	GLuint m_modelMatrixUniform;

	pagoda::math::Mat4x4F m_projectionMatrix;
	pagoda::math::Mat4x4F m_viewMatrix;
	pagoda::math::Mat4x4F m_modelMatrix;
};
}  // namespace pgeditor::renderer::gl
