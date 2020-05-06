#ifndef PAGODA_MATH_LIB_LINE_SEGMENT_3D_H_
#define PAGODA_MATH_LIB_LINE_SEGMENT_3D_H_

#include "vec_base.h"

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace pagoda
{
template<class Rep>
class LineSegment3D
{
public:
	using VectorType = boost::qvm::vec<Rep, 3>;

	/**
	 * Constructs the line segment between (0,0,0) and (1,0,0).
	 */
	LineSegment3D() : m_point{0, 0, 0}, m_supportVector{1, 0, 0} {}

	/**
	 * Constructs the line segment between \p p1 and \p p2.
	 */
	LineSegment3D(const VectorType& p1, const VectorType& p2) : m_point(p1), m_supportVector(p2 - p1) {}

	/**
	 * Returns the origin point (when t = 0).
	 */
	const VectorType& GetSourcePoint() const { return m_point; }
	/**
	 * Returns the target point (when t = 1).
	 */
	VectorType GetTargetPoint() const { return m_point + m_supportVector; }
	/**
	 * Returns the line's supporting vector.
	 */
	const VectorType& GetSupportVector() const { return m_supportVector; }

	bool operator==(const LineSegment3D<Rep>& other) const
	{
		return m_point == other.m_point && m_supportVector == other.m_supportVector;
	}

	bool operator!=(const LineSegment3D<Rep>& other) const { return !(*this == other); }

private:
	VectorType m_point;          ///< The origin point in this line segment (t == 0)
	VectorType m_supportVector;  ///< This line's supporting vector
};

}  // namespace pagoda

#endif
