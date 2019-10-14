#ifndef SELECTOR_GEOMETRY_OPERATIONS_PLANE_SPLITS_H_
#define SELECTOR_GEOMETRY_OPERATIONS_PLANE_SPLITS_H_

#include "clip.h"

namespace selector
{
template<class G>
class PlaneSplits
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

public:
	PlaneSplits(const std::vector<Plane<float>> &planes) : m_planes(planes) {}

	template<class Container>
	void Execute(GeometryPtr geometryIn, Container &outGeometries)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Plane Splits with %d planes", m_planes.size());
#ifdef DEBUG
		for (const auto &p : m_planes)
		{
			LOG_TRACE(GeometryOperations, " Plane %s", to_string(p).c_str());
		}
#endif

		outGeometries.reserve(m_planes.size() + 1);
		auto currentGeometry = std::make_shared<Geometry>();
		*currentGeometry = *geometryIn;

		for (const auto &plane : m_planes)
		{
			Clip<Geometry> clip(plane);
			auto frontGeometry = std::make_shared<Geometry>();
			auto backGeometry = std::make_shared<Geometry>();

			clip.Execute(currentGeometry, frontGeometry, backGeometry);

			outGeometries.push_back(frontGeometry);
			currentGeometry = backGeometry;
		}

		outGeometries.push_back(currentGeometry);
	}

private:
	std::vector<Plane<float>> m_planes;
};
}  // namespace selector

#endif
