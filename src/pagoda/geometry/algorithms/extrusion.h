#ifndef PAGODA_GEOMETRY_OPERATIONS_EXTRUSION_H_
#define PAGODA_GEOMETRY_OPERATIONS_EXTRUSION_H_

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/geometry/core/geometry_sizes.h>

#include <pagoda/geometry/algorithms/traverse.h>

#include <vector>

namespace pagoda::geometry::algorithms
{
/**
 * Extrudes all faces of a Geometry along its normal by a given amount.
 */
template<class G>
class Extrusion
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;
	using Index_t = typename Geometry::Index_t;

public:
	/**
	 * Defines the extrusion amount for the Extrusion.
	 */
	explicit Extrusion(const float &extrusion_amount) : m_extrusionAMount(extrusion_amount)
	{
		DBG_ASSERT_MSG(extrusion_amount != 0, "Can't extrude by zero.");
	}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Extrusion. Amount: " << m_extrusionAMount);

		core::GeometryBuilderT<Geometry> builder(geometryOut);

		std::unordered_map<Index_t, Index_t> pointsMap;

		algorithms::EachPoint(geometryIn.get(), [&pointsMap, &builder](G *geometry, const typename G::PointHandle &p) {
			pointsMap[p] = builder.AddPoint(geometry->GetPosition(p));
		});

		algorithms::EachFace(geometryIn.get(), [this, &pointsMap, &builder](G *geometry,
		                                                                    const typename G::FaceHandle &f) {
			uint32_t bottomFaceSize = 0;
			algorithms::EachPointAroundFace(
			    geometry, f, [&bottomFaceSize](G *geometry, const typename G::PointHandle &p) { ++bottomFaceSize; });

			auto extrusionVector = m_extrusionAMount * core::face_normal<Geometry>(geometry, f);
			auto bottomFace = builder.StartFace(bottomFaceSize);
			auto topFace = builder.StartFace(bottomFaceSize);
			auto sideFaces = builder.StartFaces(bottomFaceSize, 4);

			std::vector<Index_t> bottomIndices;
			bottomIndices.reserve(bottomFaceSize);
			std::vector<Index_t> topIndices;
			topIndices.reserve(bottomFaceSize);

			algorithms::EachPointAroundFace(geometry, f,
			                                [&extrusionVector, &builder, &bottomIndices, &topIndices, &pointsMap](
			                                    G *geometry, const typename G::PointHandle &p) {
				                                auto bottomPosition = geometry->GetPosition(p);
				                                auto topPosition = bottomPosition + extrusionVector;

				                                bottomIndices.push_back(pointsMap[p]);
				                                topIndices.push_back(builder.AddPoint(topPosition));
			                                });

			for (auto i = 0u; i < topIndices.size(); ++i)
			{
				topFace.AddIndex(topIndices[i]);
			}

			for (auto i = 0u; i < topIndices.size(); ++i)
			{
				auto &sideFace = sideFaces[i];
				sideFace.AddIndex(bottomIndices[i]);
				sideFace.AddIndex(bottomIndices[(i + 1) % bottomFaceSize]);
				sideFace.AddIndex(topIndices[(i + 1) % bottomFaceSize]);
				sideFace.AddIndex(topIndices[i]);
			}

			for (int32_t i = bottomIndices.size() - 1; i >= 0; --i)
			{
				bottomFace.AddIndex(bottomIndices[i]);
			}

			bottomFace.CloseFace();
			for (auto &f : sideFaces)
			{
				f.CloseFace();
			}
			topFace.CloseFace();
		});
	}

private:
	/// The extrusion amount.
	float m_extrusionAMount;
};  // class Extrusion
}  // namespace pagoda::geometry::algorithms
#endif
