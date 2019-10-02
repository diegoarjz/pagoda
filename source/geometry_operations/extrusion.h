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
	using IndexType = typename Geometry::Index_t;

public:
	/**
	 * Defines the extrusion amount for the Extrusion.
	 */
	explicit Extrusion(const float &extrusion_amount) : m_extrusionAMount(extrusion_amount)
	{
		DBG_ASSERT_MSG(extrusion_amount != 0, "Can't extrude by zero.");
	}

	GeometrySizes ResultSize(GeometryPtr geometryIn) const
	{
		return GeometrySizes(0,0,0);
	}

	void Execute(GeometryPtr geometryIn, GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "Extrusion. Amount: %f", m_extrusionAMount);

		GeometryBuilderT<Geometry> builder(geometryOut);

        /*
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
        */
	}

private:
	/// The extrusion amount.
	float m_extrusionAMount;
};  // class Extrusion
}  // namespace selector
#endif
