#include "planar_texture_projection.h"

#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

using namespace boost::qvm;

namespace pagoda::geometry::core
{
PlanarTextureProjection::PlanarTextureProjection(const vec<float, 3> &origin, const vec<float, 3> &uAxis,
                                                 const vec<float, 3> &vAxis)
    : m_origin(origin), m_uAxis(uAxis), m_vAxis(vAxis), m_offset{0, 0}, m_scale{1, 1}, m_clamp{false}
{
}

void PlanarTextureProjection::SetOffset(const float &u, const float &v)
{
	X(m_offset) = u;
	Y(m_offset) = v;
}
void PlanarTextureProjection::SetScale(const float &u, const float &v)
{
	X(m_scale) = u;
	Y(m_scale) = v;
}
void PlanarTextureProjection::SetClamp(bool clamp) { m_clamp = clamp; }

vec<float, 2> PlanarTextureProjection::GetProjection(const vec<float, 3> &p)
{
	// clang-format off
	vec<float, 2> projected{
        dot(m_uAxis, p-m_origin) / X(m_scale),
        dot(m_vAxis, p-m_origin) / Y(m_scale)
    };
	// clang-format on
	projected -= m_offset;

	if (m_clamp)
	{
		X(projected) = std::min(1.0f, std::max(0.0f, X(projected)));
		Y(projected) = std::min(1.0f, std::max(0.0f, Y(projected)));
	}

	return projected;
}
}  // namespace pagoda::geometry::core
