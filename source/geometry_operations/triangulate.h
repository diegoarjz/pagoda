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
	using IndexType = typename Geometry::IndexType;

public:
	Triangulate() {}

	GeometrySizes ResultSize(GeometryPtr geometryIn) const
	{
		uint32_t numVerts = geometryIn->GetNumVertices();
		uint32_t numFaces = 0;

		auto faceEnd = geometryIn->FacesEnd();
		for (auto faceIter = geometryIn->FacesBegin(); faceIter != faceEnd; ++faceIter)
		{
			auto faceVertexSize = geometryIn->FaceVertexSize(*faceIter);
			numFaces += faceVertexSize - 2;
		}

		return GeometrySizes(numVerts, numFaces * 3, numFaces);
	}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;

		LOG_TRACE(GeometryOperations, "Triangulate.");
		LOG_TRACE(GeometryOperations, "\tGeometry In. #Vertices: %d #Edges: %d #Faces: %d",
		          geometryIn->GetNumVertices(), geometryIn->GetNumEdges(), geometryIn->GetNumFaces());
		LOG_TRACE(GeometryOperations, "\tGeometry Out. Reserved #Vertices: %d #Edges: %d #Faces: %d",
		          geometryOut->GetReservedVertices(), geometryOut->GetReservedEdges(), geometryOut->GetReservedFaces());

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
	}
};
}  // namespace selector

#endif
