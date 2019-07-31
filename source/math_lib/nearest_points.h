#ifndef SELECTOR_MATH_LIB_NEAREST_POINTS_H_
#define SELECTOR_MATH_LIB_NEAREST_POINTS_H_

namespace selector
{
template<class P>
struct NearestPoints
{
	P m_point1;
	P m_point2;
};

template<class Rep>
NearestPoints<VecBase<3, Rep>> nearest_points(const Line3<Rep> &l1, const Line3<Rep> &l2)
{
}

template<class Rep>
NearestPoints<VecBase<3, Rep>> nearest_points(const Line3<Rep> &l1, const LineSegment3<Rep> &l2)
{
}

template<class Rep>
NearestPoints<VecBase<3, Rep>> nearest_points(const LineSegment3<Rep> &l1, const Line3<Rep> &l2)
{
	auto n = nearest_points(l2, l1);
	std::swap(n.m_point1, n.m_point2);
	return n;
}
}
#endif

