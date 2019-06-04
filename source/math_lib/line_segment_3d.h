#ifndef SELECTOR_MATH_LIB_LINE_SEGMENT_3D_H_
#define SELECTOR_MATH_LIB_LINE_SEGMENT_3D_H_

#include "vector_base.h"
#include "normalize.h"

namespace selector
{
template<class Rep>
class LineSegment3D
{
public:
    using VectorType = VecBase<3, Rep>;

    /**
     * Constructs the line segment between (0,0,0) and (1,0,0).
     */
    LineSegment3D() : m_point(0,0,0), m_supportVector(1,0,0) {}
    
    /**
     * Constructs the line segment between \p p1 and \p p2.
     */
    LineSegment3D(const VectorType &p1, const VectorType &p2) : m_point(p1), m_supportVector(p2-p1) {}
    
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
private:
    VectorType m_point;         ///< The origin point in this line segment (t == 0)
    VectorType m_supportVector; ///< This line's supporting vector
};

}

#endif