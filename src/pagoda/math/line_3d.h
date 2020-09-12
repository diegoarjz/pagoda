#ifndef PAGODA_MATH_LIB_LINE_3D_H_
#define PAGODA_MATH_LIB_LINE_3D_H_

#include "vec_base.h"

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace pagoda::math
{
template<class Rep>
class Line3D
{
	public:
	using VectorType = boost::qvm::vec<Rep, 3>;

	/**
	 * Constructs and returns a \c Line3D going through \p p1 and \p p2 and has the
	 * normalized direction ||(\p p2 - \p p1)||
	 */
	static Line3D<Rep> FromTwoPoints(const VectorType& p1, const VectorType& p2)
	{
		return Line3D<Rep>(p1, boost::qvm::normalized(p2 - p1));
	}

	/**
	 * Constructs the x axis.
	 */
	Line3D() : m_point{0, 0, 0}, m_direction{1, 0, 0} {}

	/**
	 * Constructs the 3D line going through the \p point and with the \p normalizedDirection direction.
	 */
	Line3D(const VectorType& point, const VectorType& normalizedDirection)
	  : m_point(point), m_direction(normalizedDirection)
	{
	}

	/**
	 * Returns a point this line goes through.
	 */
	const VectorType& GetPoint() const { return m_point; }
	/**
	 * Returns the normalized direction of this line.
	 */
	const VectorType& GetSupportVector() const { return m_direction; }

	bool operator==(const Line3D<Rep>& other) const
	{
		return m_point == other.m_point && m_direction == other.m_direction;
	}

	bool operator!=(const Line3D<Rep>& other) const { return !(*this == other); }

	private:
	VectorType m_point;      ///< A point in this line
	VectorType m_direction;  ///< The line's normalized direction
};

}  // namespace pagoda::math

#endif
