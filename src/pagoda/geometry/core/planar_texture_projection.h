#pragma once

#include <pagoda/math/vec_base.h>

namespace pagoda::geometry::core
{
class PlanarTextureProjection
{
public:
	PlanarTextureProjection(const boost::qvm::vec<float, 3> &origin, const boost::qvm::vec<float, 3> &uAxis,
	                        const boost::qvm::vec<float, 3> &vAxis);

	void SetOffset(const float &u, const float &v);
	void SetScale(const float &u, const float &v);
	void SetClamp(bool clamp);

	boost::qvm::vec<float, 2> GetProjection(const boost::qvm::vec<float, 3> &p);

private:
	boost::qvm::vec<float, 3> m_origin;
	boost::qvm::vec<float, 3> m_uAxis;
	boost::qvm::vec<float, 3> m_vAxis;
	boost::qvm::vec<float, 2> m_offset;
	boost::qvm::vec<float, 2> m_scale;
	bool m_clamp;
};
}  // namespace pagoda::geometry::core
