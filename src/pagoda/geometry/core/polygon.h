#pragma once

#include <pagoda/math/vec_base.h>

#include <functional>
#include <vector>

namespace pagoda::geometry::core
{
template<class T>
class Polygon
{
public:
	Polygon() {}
	Polygon(const std::vector<T>&& points) : m_points(std::move(points)) {}
	Polygon(Polygon<T>&& p) : m_points(std::move(p.m_points)) {}
	Polygon(Polygon<T>& p) : m_points(p.m_points) {}

	Polygon& operator=(const Polygon& o)
	{
		m_points = o.m_points;
		return *this;
	}

	std::size_t GetPointCount() const { return m_points.size(); }

	const std::vector<T>& GetPoints() const { return m_points; }

	void Transform(const std::function<T(const T&)>& f)
	{
		for (auto& p : m_points)
		{
			p = f(p);
		}
	}

	bool operator==(const Polygon& o) const
	{
		if (GetPointCount() != o.GetPointCount())
		{
			return false;
		}

		for (auto i = 0u; i < GetPointCount(); ++i)
		{
			if (m_points[i] != o.m_points[i])
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const Polygon& o) { return !(*this == o); }

private:
	std::vector<T> m_points;
};

using Polygon2 = Polygon<math::Vec2F>;
using Polygon3 = Polygon<math::Vec3F>;

}  // namespace pagoda::geometry::core
