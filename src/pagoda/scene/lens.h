#pragma once

#include <pagoda/math/matrix_base.h>

namespace pagoda::scene
{
class Lens
{
	public:
	Lens();

	void SetPerspective(float fovY, float aspect, float near, float far);
	void SetOrthogonal(float left, float right, float top, float bottom);

	const boost::qvm::mat<float, 4, 4>& GetProjectionMatrix() const;

	protected:
	pagoda::math::Mat4x4F m_projectionMatrix;
};
}  // namespace pagoda::scene
