#ifndef PAGODA_GEOMETRY_OPERATIONS_PLANE_SPLITS_H_
#define PAGODA_GEOMETRY_OPERATIONS_PLANE_SPLITS_H_

#include "clip.h"

namespace pagoda::geometry::algorithms
{
template<class G>
class PlaneSplits
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

public:
	PlaneSplits(const std::vector<math::Plane<float>> &planes) : m_planes(planes) {}

	template<class Container>
	void Execute(GeometryPtr geometryIn, Container &outGeometries)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Plane Splits with " << m_planes.size() << " planes");

		outGeometries.reserve(m_planes.size() + 1);
		auto currentGeometry = std::make_shared<Geometry>();
		*currentGeometry = *geometryIn;

		for (const auto &plane : m_planes)
		{
			Clip<Geometry> clip(plane);
			auto frontGeometry = std::make_shared<Geometry>();
			auto backGeometry = std::make_shared<Geometry>();

			clip.Execute(currentGeometry, frontGeometry, backGeometry);

			if (frontGeometry->GetFaceCount() > 0)
			{
				outGeometries.push_back(frontGeometry);
			}
			currentGeometry = backGeometry;
		}

		if (currentGeometry->GetFaceCount() > 0)
		{
			outGeometries.push_back(currentGeometry);
		}
	}

private:
	std::vector<math::Plane<float>> m_planes;
};
}  // namespace pagoda::geometry::algorithms

#endif