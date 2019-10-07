#ifndef SELECTOR_GEOMETRY_OPERATIONS_TRIANGULATE_H_
#define SELECTOR_GEOMETRY_OPERATIONS_TRIANGULATE_H_

#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_sizes.h>

#include <common/logger.h>

#include <cstdint>
#include <memory>

#include <iostream>

namespace selector
{
template<class G>
class Triangulate
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

public:
	Triangulate() {}

	GeometrySizes ResultSize(GeometryPtr geometryIn) const { return GeometrySizes(0, 0, 0); }

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;

		LOG_TRACE(GeometryOperations, "Triangulate.");

		GeometryBuilderT<Geometry> builder(geometryOut);

		std::unordered_map<Index_t, Index_t> pointsMap;

		auto pointEndIter = geometryIn->PointsEnd();
		for (auto pointIter = geometryIn->PointsBegin(); pointIter != pointEndIter; ++pointIter)
		{
			auto vertPos = geometryIn->GetPosition(*pointIter);
			pointsMap[*pointIter] = builder.AddPoint(vertPos);
		}

		for (auto faceIter = geometryIn->FacesBegin(); faceIter != geometryIn->FacesEnd(); ++faceIter)
		{
			std::vector<Index_t> indices;
			for (auto fpCirc = geometryIn->FacePointCirculatorBegin(*faceIter); fpCirc; ++fpCirc)
			{
				indices.push_back(pointsMap[*fpCirc]);
			}

			for (uint32_t i = 2; i < indices.size(); ++i)
			{
				auto faceBuilder = builder.StartFace(3);
				faceBuilder.AddIndex(indices[0]);
				faceBuilder.AddIndex(indices[i - 1]);
				faceBuilder.AddIndex(indices[i]);
				faceBuilder.CloseFace();
			}
		}
	}
};
}  // namespace selector

#endif
