#ifndef PAGODA_MATH_LIB_NEAREST_POINTS_H_
#define PAGODA_MATH_LIB_NEAREST_POINTS_H_

#include "line_3d.h"
#include "line_segment_3d.h"
#include "vec_base.h"

#include <boost/qvm/vec_operations.hpp>

#include <cmath>

namespace pagoda
{
template<class P>
struct NearestPoints
{
	P m_point1;
	P m_point2;
};

template<class Rep>
NearestPoints<boost::qvm::vec<Rep, 3>> nearest_points(const Line3D<Rep> &l1, const Line3D<Rep> &l2)
{
	NearestPoints<boost::qvm::vec<Rep, 3>> n;
	auto p1 = l1.GetPoint();
	auto p2 = p1 + l1.GetSupportVector();
	auto p3 = l2.GetPoint();
	auto p4 = p3 + l2.GetSupportVector();
	auto p13 = p1 - p3;
	auto p43 = p4 - p3;

	if (boost::qvm::mag_sqr(p43) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	auto p21 = p2 - p1;
	if (boost::qvm::mag_sqr(p2) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	Rep d1343 = boost::qvm::dot(p13, p43);
	Rep d4321 = boost::qvm::dot(p43, p21);
	Rep d1321 = boost::qvm::dot(p13, p21);
	Rep d4343 = boost::qvm::dot(p43, p43);
	Rep d2121 = boost::qvm::dot(p21, p21);

	Rep denom = d2121 * d4343 - d4321 * d4321;
	if (std::abs(denom) <= Rep(0))
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}
	Rep numer = d1343 * d4321 - d1321 * d4343;

	Rep mua = numer / denom;
	Rep mub = (d1343 + d4321 * (mua)) / d4343;

	n.m_point1 = p1 + mua * p21;
	n.m_point2 = p3 + mub * p43;

	return n;
}

template<class Rep>
NearestPoints<boost::qvm::vec<Rep, 3>> nearest_points(const Line3D<Rep> &l1, const LineSegment3D<Rep> &l2)
{
	NearestPoints<boost::qvm::vec<Rep, 3>> n;
	auto p1 = l1.GetPoint();
	auto p2 = p1 + l1.GetSupportVector();
	auto p3 = l2.GetSourcePoint();
	auto p4 = p3 + l2.GetSupportVector();
	auto p13 = p1 - p3;
	auto p43 = p4 - p3;

	if (boost::qvm::mag_sqr(p43) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	auto p21 = p2 - p1;
	if (boost::qvm::mag_sqr(p2) <= 0)
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}

	Rep d1343 = boost::qvm::dot(p13, p43);
	Rep d4321 = boost::qvm::dot(p43, p21);
	Rep d1321 = boost::qvm::dot(p13, p21);
	Rep d4343 = boost::qvm::dot(p43, p43);
	Rep d2121 = boost::qvm::dot(p21, p21);

	Rep denom = d2121 * d4343 - d4321 * d4321;
	if (std::abs(denom) <= Rep(0))
	{
		n.m_point1 = p1;
		n.m_point2 = p1;
		return n;
	}
	Rep numer = d1343 * d4321 - d1321 * d4343;

	Rep mua = std::max(Rep(0), std::min(Rep(1), numer / denom));
	Rep mub = std::max(Rep(0), std::min(Rep(1), (d1343 + d4321 * (mua)) / d4343));

	n.m_point1 = p1 + mua * p21;
	n.m_point2 = p3 + mub * p43;

	return n;
}

template<class Rep>
NearestPoints<boost::qvm::vec<Rep, 3>> nearest_points(const LineSegment3D<Rep> &l1, const Line3D<Rep> &l2)
{
	auto n = nearest_points(l2, l1);
	std::swap(n.m_point1, n.m_point2);
	return n;
}
}  // namespace pagoda
#endif

