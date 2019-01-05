#ifndef SELECTOR_GEOMETRY_OPERATIONS_EXTRUSION_H_
#define SELECTOR_GEOMETRY_OPERATIONS_EXTRUSION_H_

#include <common/assertions.h>
#include <common/profiler.h>
#include <geometry_core/geometry.h>
#include <geometry_core/geometry_builder.h>
#include <geometry_core/geometry_sizes.h>

#include <vector>

namespace selector
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
	using IndexType = typename Geometry::IndexType;

public:
	/**
	 * Defines the extrusion amount for the Extrusion.
	 */
	explicit Extrusion(const float &extrusion_amount) : m_extrusionAMount(extrusion_amount)
	{
		DBG_ASSERT_MSG(extrusion_amount != 0, "Can't extrude by zero.");
	}

	// TODO Fix here
	GeometrySizes ResultSize(GeometryPtr geometryIn) const
	{
		auto in_num_points = geometryIn->GetNumVertices();
		size_t out_edge_count = 0u;
		size_t out_num_points = static_cast<size_t>(in_num_points);
		size_t out_face_count = 0u;
		size_t num_side_faces = 0u;

		auto face_end = geometryIn->FacesEnd();
		uint32_t bottom_face_index = 0;
		for (auto face_iter = geometryIn->FacesBegin(); face_iter != face_end; ++face_iter, ++bottom_face_index)
		{
			auto face_vertex_size = geometryIn->FaceVertexSize(*face_iter);

			out_edge_count += 6 * face_vertex_size;

			/*
			 * Number of output points is calculated by
			 *  points in original geometry + face num points
			 */
			out_num_points += face_vertex_size;

			out_face_count += 2 + face_vertex_size;

			num_side_faces += face_vertex_size;
		}

		return GeometrySizes(out_num_points, out_edge_count, out_face_count);
	}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Extrusion. Amount: %f", m_extrusionAMount);
		LOG_TRACE(GeometryOperations, "\tGeometry In. #Vertices: %d #Edges: %d #Faces: %d",
		          geometryIn->GetNumVertices(), geometryIn->GetNumEdges(), geometryIn->GetNumFaces());
		LOG_TRACE(GeometryOperations, "\tGeometry Out. Reserved #Vertices: %d #Edges: %d #Faces: %d",
		          geometryOut->GetReservedVertices(), geometryOut->GetReservedEdges(), geometryOut->GetReservedFaces());

		GeometryBuilderT<Geometry> builder(geometryOut);

		auto in_num_points = geometryIn->GetNumVertices();
		std::vector<Vec3F> in_points;
		in_points.reserve(in_num_points);
		for (auto vert_iter = geometryIn->VerticesBegin(); vert_iter != geometryIn->VerticesEnd(); ++vert_iter)
		{
			auto vert_position = geometryIn->GetVertexAttributes(*vert_iter).m_position;
			in_points.push_back(vert_position);
			builder.AddPoint(vert_position);
		}

		auto f_iter_end = geometryIn->FacesEnd();
		for (auto f_iter = geometryIn->FacesBegin(); f_iter != f_iter_end; ++f_iter)
		{
			uint32_t bottom_face_size = geometryIn->FaceVertexSize(*f_iter);
			auto extrusion_vector = m_extrusionAMount * face_normal<Geometry>(geometryIn, *f_iter);
			auto bottom_face = builder.StartFace(bottom_face_size);
			auto top_face = builder.StartFace(bottom_face_size);
			auto side_faces = builder.StartFaces(bottom_face_size, 4);

			std::vector<IndexType> bottom_indices;
			bottom_indices.reserve(bottom_face_size);
			std::vector<IndexType> top_indices;
			top_indices.reserve(bottom_face_size);

			for (auto f_v_circ = geometryIn->FaceVertexBegin(*f_iter); f_v_circ.IsValid(); ++f_v_circ)
			{
				auto bottom_position = geometryIn->GetVertexAttributes(*f_v_circ).m_position;
				auto top_position = bottom_position + extrusion_vector;

				bottom_indices.push_back(*f_v_circ);
				top_indices.push_back(builder.AddPoint(top_position));
			}

			for (uint32_t i = 0; i < top_indices.size(); ++i)
			{
				top_face.AddIndex(top_indices[i]);
			}

			for (uint32_t i = 0; i < top_indices.size(); ++i)
			{
				auto &side_face = side_faces[i];
				side_face.AddIndex(bottom_indices[i]);
				side_face.AddIndex(bottom_indices[(i + 1) % bottom_face_size]);
				side_face.AddIndex(top_indices[(i + 1) % bottom_face_size]);
				side_face.AddIndex(top_indices[i]);
			}

			for (int32_t i = bottom_indices.size() - 1; i >= 0; --i)
			{
				bottom_face.AddIndex(bottom_indices[i]);
			}

			bottom_face.CloseFace();
			for (auto &f : side_faces)
			{
				f.CloseFace();
			}
			top_face.CloseFace();
		}
	}

private:
	/// The extrusion amount.
	float m_extrusionAMount;
};  // class Extrusion
}  // namespace selector
#endif
