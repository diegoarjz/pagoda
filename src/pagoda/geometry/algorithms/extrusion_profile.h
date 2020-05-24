#ifndef PAGODA_GEOMETRY_OPERATIONS_EXTRUSION_PROFILE_H_
#define PAGODA_GEOMETRY_OPERATIONS_EXTRUSION_PROFILE_H_

#include <vector>

#include <math_lib/VecBase.h>

namespace pagoda::geometry::algorithms
{
/**
 * Implements an extrusion profile that is to be applied to
 * face edges when extruding.
 *
 * For each point in the profile, the x value indicates the displacement towards the outside (positive) or inside
 * (negative) of the face, while the y value indicates the distance from the plane containing the face. Y values
 * should be added in increasing order. A value of (0,0) indicates the edge position.
 */
class ExtrusionProfile
{
public:
	ExtrusionProfile(const size_t &num_points);

	void AddPoint(const math_lib::boost::qvm::vec<float, 2> &point);

	std::vector<math_lib::boost::qvm::vec<float, 2>>::const_iterator cbegin() { return profile.cbegin(); }
	std::vector<math_lib::boost::qvm::vec<float, 2>>::const_iterator cend() { return profile.cend(); }

private:
	std::vector<math_lib::boost::qvm::vec<float, 2>> profile;
};  // class ExtrusionProfile
}  // namespace pagoda::geometry::algorithms
#endif
