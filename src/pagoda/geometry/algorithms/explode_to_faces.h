#pragma once

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>

namespace pagoda::geometry::algorithms
{
template<class G>
class ExplodeToFaces
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;

public:
	ExplodeToFaces() {}

	template<class Container>
	void Execute(GeometryPtr geometryIn, Container &outGeometries)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Exploding to faces");

		outGeometries.reserve(geometryIn->GetFaceCount());

		for (auto fIter = geometryIn->FacesBegin(); fIter != geometryIn->FacesEnd(); ++fIter)
		{
			auto faceGeometry = std::make_shared<Geometry>();
			core::GeometryBuilderT<Geometry> builder(faceGeometry);

			auto faceBuilder = builder.StartFace();
			for (auto fpCirc = geometryIn->FacePointCirculatorBegin(*fIter); fpCirc; ++fpCirc)
			{
				faceBuilder.AddIndex(builder.AddPoint(geometryIn->GetPosition(*fpCirc)));
			}
			faceBuilder.CloseFace();

			outGeometries.push_back(faceGeometry);
		}
	}
};
}  // namespace pagoda::geometry::algorithms
