#ifndef SELECTOR_MATH_LIB_PLANE_H_
#define SELECTOR_MATH_LIB_PLANE_H_

#include "cross_product.h"
#include "vec_base.h"

namespace selector
{
template<class Rep>
class Plane
{
public:
	using VectorType = VecBase<3, Rep>;
	using PointType = VecBase<3, Rep>;

	/**
	 * Constructs the xy-plane;
	 */
	Plane() : m_normal(0, 0, 1), m_distance(0) {}
	/**
	 * Constructs a plane with the \p normal and \p distance.
	 */
	Plane(const VectorType &normalizedNormal, const Rep &distance) : m_normal(normalizedNormal), m_distance(distance) {}

	/**
	 * Static constructor with a point and a normal.
	 */
	static Plane FromPointAndNormal(const PointType &p, const VectorType &n) { return Plane(normalized(n), length(p)); }

	/**
	 * Static constructor from three points.
	 * Constructs the plane passing through \p p1 and with the normal defined by
	 * (\p p1 - \p p)x(\p p2 - \p p)
	 */
	static Plane FromPoints(const PointType &p, const PointType &p1, const PointType &p2)
	{
		return Plane(normalized(cross_product(p1 - p, p2 - p)), length(p));
	}

	/**
	 * Static constructor from a point and two vectors contained in the plane.
	 * Constructs the plane passing through \p p and with normal defined by \p v1 x \p v2
	 */
	static Plane FromPointVectors(const PointType &p, const VectorType &v1, const VectorType &v2)
	{
		return Plane(normalized(cross_product(v1, v2)), length(p));
	}

	/**
	 * Returns the plane normal.
	 */
	const VectorType &GetNormal() const { return m_normal; }

	/**
	 * Returns the distance between the plane and the origin.
	 */
	const Rep &GetDistanceToOrigin() const { return m_distance; }

	/**
	 * Returns a point in the plane.
	 */
	PointType Point() const { return m_distance * m_normal; }

private:
	VectorType m_normal;  ///< Plane Normal
	Rep m_distance;       ///< Distance to origin
};
}  // namespace selector

#endif
