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
	using IndexType = typename Geometry::Index_t;

public:
	Triangulate() {}

	GeometrySizes ResultSize(GeometryPtr geometryIn) const
	{
		return GeometrySizes(0,0,0);
	}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;

		LOG_TRACE(GeometryOperations, "Triangulate.");

        /*
		GeometryBuilderT<Geometry> builder(geometryOut);

		auto vertIterEnd = geometryIn->VerticesEnd();
		for (auto vertIter = geometryIn->VerticesBegin(); vertIter != vertIterEnd; ++vertIter)
		{
			auto vertPos = geometryIn->GetVertexAttributes(*vertIter).m_position;
			builder.AddPoint(vertPos);
		}

		auto faceIterEnd = geometryIn->FacesEnd();
		for (auto faceIter = geometryIn->FacesBegin(); faceIter != faceIterEnd; ++faceIter)
		{
			std::vector<IndexType> indices;
			for (auto faceVCirc = geometryIn->FaceVertexBegin(*faceIter); faceVCirc.IsValid(); ++faceVCirc)
			{
				indices.push_back(*faceVCirc);
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
        */
	}
};
}  // namespace selector

#endif
