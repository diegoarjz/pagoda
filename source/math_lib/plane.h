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
	static Plane FromPointAndNormal(const PointType &p, const VectorType &n)
	{
		auto normal = normalized(n);
		auto distanceToOrigin = dot_product(normal, p);
		return Plane(normal, distanceToOrigin);
	}

	/**
	 * Static constructor from three points.
	 * Constructs the plane passing through \p p1 and with the normal defined by
	 * (\p p1 - \p p)x(\p p2 - \p p)
	 */
	static Plane FromPoints(const PointType &p, const PointType &p1, const PointType &p2)
	{
		auto normal = normalized(cross_product(p1 - p, p2 - p));
		auto distanceToOrigin = dot_product(normal, p);
		return Plane(normal, distanceToOrigin);
	}

	/**
	 * Static constructor from a point and two vectors contained in the plane.
	 * Constructs the plane passing through \p p and with normal defined by \p v1 x \p v2
	 */
	static Plane FromPointVectors(const PointType &p, const VectorType &v1, const VectorType &v2)
	{
		auto normal = normalized(cross_product(v1, v2));
		auto distanceToOrigin = dot_product(normal, p); 
		return Plane(normal, distanceToOrigin);
	}

	/**
	 * Returns the plane normal.
	 */
	const VectorType &GetNormal() const { return m_normal; }
	
	/**
	 * Returns a vector orthogonal to the normal.
	 * The returned vector may not be normalized.
	 */
	VectorType GetVector() const { return orthogonal(m_normal); }
	 
	 /**
	  * Returns a vector that is orthogonal to the one return by GetNormal() and GetVector().
	  * The returned vector may not be normalized.
	  */
	 VectorType GetVector2() const { return cross_product(GetVector(), GetNormal()); }

	/**
	 * Returns the distance between the plane and the origin.
	 */
	const Rep &GetDistanceToOrigin() const { return m_distance; }

	/**
	 * Returns a point in the plane.
	 */
	PointType GetPoint() const { return m_distance * m_normal; }
	
	/**
	 * Returns a second point in the Plane.
	 */
	PointType GetPoint2() const { return Point() + GetVector(); }

private:
	VectorType m_normal;  ///< Plane Normal
	Rep m_distance;       ///< Distance to origin
};
}  // namespace selector

#endif
