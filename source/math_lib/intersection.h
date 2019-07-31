#ifndef SELECTOR_MATH_LIB_INTERSECTIONS_H_
#define SELECTOR_MATH_LIB_INTERSECTIONS_H_

namespace selector
{
template<class T, typename I>
struct Intersection
{
	using IntersectionType = T::Type;
	using IntersectionValue = I;
	
	IntersectionType m_type;
	IntersectionValue m_intersection;
	
	bool HasIntersection() const
	{
		return T::Intersects(m_type);
	}
};

struct PlaneIntersectionType
{
	enum class Type
	{
		NoIntersection,
		Coplanar,
		Intersection
	};
	
	static bool Intersects(const Type &t)
	{
		return t == Intersection;
	}
};

template<class Rep>
Intersection<Line3D<Rep>, PlaneIntersectionType> intersection(const Plane<Rep> &p1, const Plane<Rep> &p2)
{
	VecBase<3, Rep> plane3Normal = cross(p1.GetNormal(), p2.GetNormal());
	Rep det = squared_length(plane3Normal);
	
	PlaneIntersectionType i;
	if (det == 0)
	{
		if (dot(p1.GetNormal(), (p2.GetPoint() - p1.GetPoint()) == 0)
		{
			// Coplanar
			i.m_type = PlaneIntersectionType::Type::Coplanar;
			i.m_intersection = p1.GetPoint();
		}
		else
		{
			// No intersection
			i.m_type = PlaneIntersectionType::Type::NoIntersection;
		}
		
		return i;
	}
	
	VecBase<3, Rep> linePoint = ((cross(plane3Normal, p1.GetNormal()) * p1.GetDistanceToOrigin()) +
								 (cross(p1.GetNormal(), plane3Normal) * p2.GetDistanceToOrigin())) / det;
	Line3D<Rep> intersectionLine(linePoint, normalized(plane3Normal));
	
	i.m_type = PlaneIntersectionType::Type::Intersection;
	i.m_intersection = intersectionLine;
	
	return i;
}

struct PlaneLineIntersection
{
	enum class Type
	{
		NoIntersection,
		Coplanar,
		Intersection
	};
	
	static bool Intersects(const Type &t)
	{
		return t == Intersection;
	}
};

template<class Rep>
Intersection<VecBase<3, Rep>, PlaneLineIntersection> intersection(const Plane<Rep> &p, const Line3D<Rep> &l)
{
	auto planeNormal = p.GetNormal();
	auto planePoint = p.GetPoint();
	auto linePoint = l.GetPoint();
	auto lineVector = l.GetSupportVector();
	
	Rep denominator = dot(planeNormal, lineVector);
	Intersection<VecBase<3, Rep>> i;
	
	if (denominator == 0)
	{
		// Plane and line are parallel
		if (dot(planePoint - linePoint, planeNormal) == 0)
		{
			// Coplanar
			i.m_type = PlaneLineIntersection::Type::Coplanar;
			i.m_intersection = linePoint;
		}
		else
		{
			// Simply parallel
			i.m_type = PlaneLineIntersection::Type::NoIntersection;
		}
		return i;
	}
	
	auto u = dot(planeNormal, linePoint) / denominator;
	i.m_type = PlaneLineIntersection::Type::Intersection;
	i.m_intersection = linePoint + lineVector * u;
	return i;
}

template<class Rep>
Intersection<VecBase<3, Rep>, PlaneLineIntersection> intersection(const Line3D<Rep> &l, const Plane<Rep> &p(
{
	return intersection(p, l);
}
}

#endif
