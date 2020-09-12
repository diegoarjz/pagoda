#pragma once

#include <pagoda/math/vec_base.h>

namespace pagoda::geometry::core
{
class PlanarTextureProjection
{
	public:
	PlanarTextureProjection(const math::Vec3F &origin, const math::Vec3F &uAxis, const math::Vec3F &vAxis);

	void SetOffset(const float &u, const float &v);
	void SetScale(const float &u, const float &v);
	void SetClamp(bool clamp);

	math::Vec2F GetProjection(const math::Vec3F &p);

	private:
	math::Vec3F m_origin;
	math::Vec3F m_uAxis;
	math::Vec3F m_vAxis;
	math::Vec2F m_offset;
	math::Vec2F m_scale;
	bool m_clamp;
};
}  // namespace pagoda::geometry::core
