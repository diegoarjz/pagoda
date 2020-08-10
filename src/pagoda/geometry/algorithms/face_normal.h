#pragma once

#include "traverse.h"

#include <pagoda/math/vec_base.h>
#include <boost/qvm/gen/vec_operations2.hpp>

#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace pagoda::geometry::algorithms
{
template<class Geometry>
inline typename Geometry::PositionType face_normal(Geometry *geometry, const typename Geometry::Index_t &face)
{
	START_PROFILE;

	math::Vec3F normal{0, 0, 0};
	algorithms::EachSplitPointAroundFace(geometry, face,
	                                     [&normal](Geometry *g, const typename Geometry::SplitPointHandle &sp) {
		                                     auto curr = g->GetPosition(g->GetPoint(sp));
		                                     auto next = g->GetPosition(g->GetPoint(g->GetNextSplitPoint(sp)));
		                                     X(normal) += (Y(curr) - Y(next)) * (Z(curr) + Z(next));
		                                     Y(normal) += (Z(curr) - Z(next)) * (X(curr) + X(next));
		                                     Z(normal) += (X(curr) - X(next)) * (Y(curr) + Y(next));
	                                     });

#ifdef DEBUG
	if (boost::qvm::mag_sqr(normal) == 0)
	{
		LOG_ERROR("Normal's magnitude is 0");
		algorithms::EachPointAroundFace(geometry, face, [](Geometry *g, const typename Geometry::PointHandle &p) {
			auto pos = g->GetPosition(p);
			LOG_ERROR(" (" << X(pos) << ", " << Y(pos) << ", " << Z(pos) << ")");
		});
	}
#endif
	DBG_ASSERT_MSG(boost::qvm::mag_sqr(normal) > 0, "The normal's magnitude is 0. Are all face points collinear?");
	return boost::qvm::normalized(normal);
}

template<class Geometry>
inline typename Geometry::PositionType face_normal(std::shared_ptr<Geometry> geometry,
                                                   const typename Geometry::Index_t &face)
{
	return face_normal(geometry.get(), face);
}
}  // namespace pagoda::geometry::algorithms
