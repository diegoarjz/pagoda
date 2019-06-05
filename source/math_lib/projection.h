#ifndef SELECTOR_MATH_LIB_PROJECTION_H_
#define SELECTOR_MATH_LIB_PROJECTION_H_

namespace selector
{

/**
 * Calculates the projection of point \p p onto the line \p l
 */
template<class Rep>
VecBase<3, Rep> projection(const VecBase<3, Rep> &p, const Line3D<Rep> &l)
{
 auto dot = dot_product(p - l.GetPoint(), l.GetSupportingVector());
 return l.GetPoint() + l.GetSupportingVector() * dot;
}

/**
 * Calculates the projection of point \p p onto the line segment \p ls.
 * If no orthogonal projection exists from the point to the line segment, then one of the endpoints in \p ls is used.
 */
template<class Rep>
VecBase<3, Rep> projection(const VecBase<3, Rep> &p, const LineSegment3D<Rep> &ls)
{
 auto dot = dot_product(p - ls.GetSourcePoint(), ls.GetSupportingVector());
 if (dot <= 0)
 {
  return ls.GetSourcePoint();
 }
 if (dot >= 1)
 {
  return ls.GetTargetPoint();
 }
 return ls.GetSourcePoint() + ls.GetSupportingVector() * dot; 
}

/**
 * Calculates the projection of line segment \p ls onto line \p l.
 * Projects both line segment endpoints onto the line.
 */
template<class Rep>
LineSegment3D<Rep> projection(const LineSegment3D<Rep> &ls, const Line3D<Rep> &l)
{
 return LineSegment3D<Rep>(projection(ls.GetSourcePoint(), l), projection(ls.GetTargetPoint(), l));
}
}

#endif