#ifndef SELECTOR_MATH_LIB_PLANE_H_
#define SELECTOR_MATH_LIB_PLANE_H_

#include "orthogonal.h"
#include "vec_base.h"

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace selector
{
template<class Rep>
class Plane
{
public:
	using VectorType = boost::qvm::vec<Rep, 3>;
	using PointType = boost::qvm::vec<Rep, 3>;

	/**
	 * Constructs the xy-plane;
	 */
	Plane() : m_normal{0, 0, 1}, m_distance{0} {}

	/**
	 * Constructs a plane with the \p normal and \p distance.
	 */
	Plane(const VectorType &normalizedNormal, const Rep &distance) : m_normal(normalizedNormal), m_distance(distance) {}

	/**
	 * Static constructor with a point and a normal.
	 */
	static Plane FromPointAndNormal(const PointType &p, const VectorType &n)
	{
		auto normal = boost::qvm::normalized(n);
		auto distanceToOrigin = boost::qvm::dot(normal, p);
		return Plane(normal, distanceToOrigin);
	}

	/**
	 * Static constructor from three points.
	 * Constructs the plane passing through \p p1 and with the normal defined by
	 * (\p p1 - \p p)x(\p p2 - \p p)
	 */
	static Plane FromPoints(const PointType &p, const PointType &p1, const PointType &p2)
	{
		auto normal = boost::qvm::normalized(boost::qvm::cross(p1 - p, p2 - p));
		auto distanceToOrigin = boost::qvm::dot(normal, p);
		return Plane(normal, distanceToOrigin);
	}

	/**
	 * Static constructor from a point and two vectors contained in the plane.
	 * Constructs the plane passing through \p p and with normal defined by \p v1 x \p v2
	 */
	static Plane FromPointAndVectors(const PointType &p, const VectorType &v1, const VectorType &v2)
	{
		auto normal = boost::qvm::normalized(boost::qvm::cross(v1, v2));
		auto distanceToOrigin = boost::qvm::dot(normal, p);
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
	VectorType GetVector2() const { return boost::qvm::cross(GetVector(), GetNormal()); }

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
	PointType GetPoint2() const { return GetPoint() + GetVector(); }

	/**
	 * Indicates on which side of the plane a geometry element is.
	 */
	enum class PlaneSide
	{
		Front,
		Back,
		Contained
	};

	/**
	 * Returns on which side of the plane the \p point is.
	 */
	PlaneSide GetPlaneSide(const VectorType &point)
	{
		auto dot = boost::qvm::dot(GetNormal(), point - GetPoint());
		if (dot == Rep(0))
		{
			return PlaneSide::Contained;
		}
		return dot > Rep(0) ? PlaneSide::Front : PlaneSide::Back;
	}

	bool operator==(const Plane<Rep> &o) const { return m_normal == o.m_normal && m_distance == o.m_distance; }
	bool operator!=(const Plane<Rep> &o) const { return !(*this == o); }

private:
	VectorType m_normal;  ///< Plane Normal
	Rep m_distance;       ///< Distance to origin
};

template<class Rep>
std::string to_string(const Plane<Rep> &plane)
{
	std::stringstream ss;
	ss << "Plane: [";
	ss << plane.GetNormal() << ", " << plane.GetDistanceToOrigin();
	ss << "]";
	return ss.str();
}

template<class Rep>
std::ostream &operator<<(std::ostream &o, const Plane<Rep> &plane)
{
	o << "Plane: [";
	o << plane.GetNormal() << ", " << plane.GetDistanceToOrigin();
	o << "]";
	return o;
}

template<class Rep>
std::string to_string(const typename Plane<Rep>::PlaneSide &side)
{
	if (side == selector::Plane<Rep>::PlaneSide::Front)
	{
		return "front";
	}
	if (side == selector::Plane<Rep>::PlaneSide::Back)
	{
		return "back";
	}
	return "contained";
}
}  // namespace selector

#endif
