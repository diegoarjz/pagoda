#pragma once

#include "traverse.h"

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
		using FaceHandle = typename Geometry::FaceHandle;
		using PointHandle = typename Geometry::PointHandle;

		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Exploding to faces");

		outGeometries.reserve(geometryIn->GetFaceCount());

		algorithms::EachFace(geometryIn.get(), [&outGeometries](Geometry *g, const FaceHandle &f) {
			auto faceGeometry = std::make_shared<Geometry>();
			core::GeometryBuilderT<Geometry> builder(faceGeometry);
			auto faceBuilder = builder.StartFace();
			algorithms::EachPointAroundFace(g, f, [&faceBuilder, &builder](Geometry *g, const PointHandle &p) {
				faceBuilder.AddIndex(builder.AddPoint(g->GetPosition(p)));
			});
			faceBuilder.CloseFace();
			outGeometries.push_back(faceGeometry);
		});
	}
};
}  // namespace pagoda::geometry::algorithms
