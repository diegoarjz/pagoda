#pragma once

#include "traverse.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>

#include <pagoda/math/bissectrix.h>

namespace pagoda::geometry::algorithms
{
template<class G>
class FaceOffset
{
	private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

	public:
	explicit FaceOffset(const float& amount) : m_amount(amount) { DBG_ASSERT_MSG(amount != 0, "Can't offset by zero."); }

	template<class T>
	void Execute(GeometryPtr geometryIn, T innerGeometries, T outerGeometries)
	{
		using FaceHandle = typename Geometry::FaceHandle;
		using SplitPointHandle = typename Geometry::SplitPointHandle;

		START_PROFILE;
		LOG_TRACE(GeometryOperations, "FaceOffset. Amount: " << m_amount);

		algorithms::EachFace(geometryIn.get(),
		                     [this, &innerGeometries, &outerGeometries](Geometry* g, const FaceHandle& f) {
			                     std::vector<math::Vec3F> outerPoints;
			                     std::vector<math::Vec3F> innerPoints;
			                     algorithms::EachSplitPointAroundFace(
			                       g, f, [this, &innerPoints, &outerPoints](Geometry* g, const SplitPointHandle& sp) {
				                       auto curr = g->GetPosition(g->GetPoint(sp));
				                       auto prev = g->GetPosition(g->GetPoint(g->GetPrevSplitPoint(sp)));
				                       auto next = g->GetPosition(g->GetPoint(g->GetNextSplitPoint(sp)));

				                       auto currBiss = m_amount * math::bissectrix((prev - curr), (next - curr));

				                       outerPoints.push_back(curr);
				                       innerPoints.push_back(curr + currBiss);
			                       });

			                     // inner geometry
			                     auto inner = std::make_shared<Geometry>();
			                     core::GeometryBuilderT innerBuilder(inner);
			                     auto face = innerBuilder.StartFace(innerPoints.size());
			                     for (const auto& i : innerPoints) {
				                     face.AddIndex(innerBuilder.AddPoint(i));
			                     }
			                     face.CloseFace();
			                     innerGeometries = inner;

			                     // outer geometries
			                     for (auto i = 0u; i < outerPoints.size(); ++i) {
				                     auto outer = std::make_shared<Geometry>();
				                     core::GeometryBuilderT outerBuilder(outer);

				                     face = outerBuilder.StartFace(4);
				                     face.AddIndex(outerBuilder.AddPoint(outerPoints[i]));
				                     face.AddIndex(outerBuilder.AddPoint(outerPoints[(i + 1) % outerPoints.size()]));
				                     face.AddIndex(outerBuilder.AddPoint(innerPoints[(i + 1) % innerPoints.size()]));
				                     face.AddIndex(outerBuilder.AddPoint(innerPoints[i]));
				                     face.CloseFace();

				                     outerGeometries = outer;
			                     }
		                     });
	}

	private:
	float m_amount;
};
}  // namespace pagoda::geometry::algorithms
