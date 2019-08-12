#ifndef SELECTOR_MATH_LIB_NEAREST_POINTS_H_
#define SELECTOR_MATH_LIB_NEAREST_POINTS_H_

#include "line_3d.h"
#include "line_segment_3d.h"
#include "vec_base.h"

#include "dot_product.h"

namespace selector
{
template<class P>
struct NearestPoints
{
	P m_point1;
	P m_point2;
};

template<class Rep>
NearestPoints<VecBase<3, Rep>> nearest_points(const Line3D<Rep> &l1, const Line3D<Rep> &l2)
{
	NearestPoints<VecBase<3, Rep>> n;
	auto p1 = l1.GetPoint();
	auto p2 = p1 + l1.GetSupportVector();
	auto p3 = l2.GetPoint();
	auto p4 = p3 + l2.GetSupportVector();
	auto p13 = p1 - p3;
	auto p43 = p4 - p3;

	if (squared_length(p43) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	auto p21 = p2 - p1;
	if (squared_length(p2) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	auto d1343 = dot_product(p13, p43);
	auto d4321 = dot_product(p43, p21);
	auto d1321 = dot_product(p13, p21);
	auto d4343 = dot_product(p43, p43);
	auto d2121 = dot_product(p21, p21);

	auto denom = d2121 * d4343 - d4321 * d4321;
	if (std::abs(denom) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}
	auto numer = d1343 * d4321 - d1321 * d4343;

	auto mua = numer / denom;
	auto mub = (d1343 + d4321 * (mua)) / d4343;

	n.m_point1 = p1 + mua * p21;
	n.m_point2 = p3 + mub * p43;

	return n;
}

template<class Rep>
NearestPoints<VecBase<3, Rep>> nearest_points(const Line3D<Rep> &l1, const LineSegment3D<Rep> &l2)
{
	NearestPoints<VecBase<3, Rep>> n;
	auto p1 = l1.GetPoint();
	auto p2 = p1 + l1.GetSupportVector();
	auto p3 = l2.GetSourcePoint();
	auto p4 = p3 + l2.GetSupportVector();
	auto p13 = p1 - p3;
	auto p43 = p4 - p3;

	if (squared_length(p43) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	auto p21 = p2 - p1;
	if (squared_length(p2) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	auto d1343 = dot_product(p13, p43);
	auto d4321 = dot_product(p43, p21);
	auto d1321 = dot_product(p13, p21);
	auto d4343 = dot_product(p43, p43);
	auto d2121 = dot_product(p21, p21);

	auto denom = d2121 * d4343 - d4321 * d4321;
	if (std::abs(denom) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}
	auto numer = d1343 * d4321 - d1321 * d4343;

	auto mua = std::max(Rep(0), std::min(Rep(1), numer / denom));
	auto mub = std::max(Rep(0), std::min(Rep(1), (d1343 + d4321 * (mua)) / d4343));

	n.m_point1 = p1 + mua * p21;
	n.m_point2 = p3 + mub * p43;

	return n;
}

template<class Rep>
NearestPoints<VecBase<3, Rep>> nearest_points(const LineSegment3D<Rep> &l1, const Line3D<Rep> &l2)
{
	auto n = nearest_points(l2, l1);
	std::swap(n.m_point1, n.m_point2);
	return n;
}
}  // namespace selector
#endif

