#pragma once

#include "lens.h"
#include "transformation.h"

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/vec_base.h>

namespace pagoda::scene
{
class Camera final
{
	public:
	Camera();

	void SetPosition(const boost::qvm::vec<float, 3> &pos);
	void SetViewDirection(const boost::qvm::vec<float, 3> &dir);
	void SetTransformation(const Transformation &t);

	const math::Vec3F &GetPosition() const;
	const math::Vec3F &GetViewDirection() const;
	math::Vec3F GetRightVector() const;
	math::Vec3F GetUpVector() const;

	const math::Mat4x4F &GetViewMatrix();
	const math::Mat4x4F &GetProjectionMatrix();

	void SetLens(const Lens &lens);
	Lens &GetLens();

	private:
	pagoda::math::Vec3F m_position;
	pagoda::math::Vec3F m_viewDirection;

	pagoda::math::Mat4x4F m_viewMatrix;
	Lens m_lens;
	bool m_viewMatrixDirty;
};
}  // namespace pagoda::scene
