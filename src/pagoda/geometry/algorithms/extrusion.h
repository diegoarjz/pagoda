#ifndef PAGODA_GEOMETRY_OPERATIONS_EXTRUSION_H_
#define PAGODA_GEOMETRY_OPERATIONS_EXTRUSION_H_

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/instrument/profiler.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/geometry/core/geometry_sizes.h>

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

		auto inNumPoints = geometryIn->GetPointCount();
		std::vector<math::Vec3F> inVertices;
		inVertices.reserve(inNumPoints);
		for (auto pointIter = geometryIn->PointsBegin(); pointIter != geometryIn->PointsEnd(); ++pointIter)
		{
			auto vertPosition = geometryIn->GetPosition(*pointIter);
			inVertices.push_back(vertPosition);
			pointsMap[*pointIter] = builder.AddPoint(vertPosition);
		}

		for (auto fIter = geometryIn->FacesBegin(); fIter != geometryIn->FacesEnd(); ++fIter)
		{
			uint32_t bottomFaceSize = 0;
			for (auto facePointIter = geometryIn->FacePointCirculatorBegin(*fIter); facePointIter; ++facePointIter)
			{
				++bottomFaceSize;
			}

			auto extrusionVector = m_extrusionAMount * core::face_normal<Geometry>(geometryIn, *fIter);
			auto bottomFace = builder.StartFace(bottomFaceSize);
			auto topFace = builder.StartFace(bottomFaceSize);
			auto sideFaces = builder.StartFaces(bottomFaceSize, 4);

			std::vector<Index_t> bottomIndices;
			bottomIndices.reserve(bottomFaceSize);
			std::vector<Index_t> topIndices;
			topIndices.reserve(bottomFaceSize);

			for (auto facePointIter = geometryIn->FacePointCirculatorBegin(*fIter); facePointIter; ++facePointIter)
			{
				auto bottomPosition = geometryIn->GetPosition(*facePointIter);
				auto topPosition = bottomPosition + extrusionVector;

				bottomIndices.push_back(pointsMap[(*facePointIter)]);
				topIndices.push_back(builder.AddPoint(topPosition));
			}

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
		}
	}

private:
	/// The extrusion amount.
	float m_extrusionAMount;
};  // class Extrusion
}  // namespace pagoda::geometry::algorithms
#endif
