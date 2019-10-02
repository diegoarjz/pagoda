#ifndef SELECTOR_GEOMETRY_CORE_GEOMETRY_EXPORTER_H_
#define SELECTOR_GEOMETRY_CORE_GEOMETRY_EXPORTER_H_

#include <ostream>

#include "geometry.h"

namespace selector
{
template<class G>
class GeometryExporter
{
protected:
	/// Convenience alias for the Geometry
	using Geometry = G;
	/// Convenience alias for a shared pointer to a Geometry
	using GeometryPtr = std::shared_ptr<Geometry>;
	/// Convenience alias for a Geometry face Index
	using IndexType = typename Geometry::Index_t;
	/// Convenience alias for a Geometry position
	using PositionType = typename Geometry::PositionType;

public:
	GeometryExporter(GeometryPtr geom) : m_geometry(geom) {}

	/**
	 * Exports the Geometry to the \p out_stream.
	 */
	bool Export(std::ostream &out_stream)
	{
        /*
		auto num_points = m_geometry->GetNumVertices();
		auto num_faces = m_geometry->GetNumFaces();

		StartGeometry(out_stream, num_points, num_faces);

		auto verts_end = m_geometry->VerticesEnd();
		for (auto vert = m_geometry->VerticesBegin(); vert != verts_end; ++vert)
		{
			PositionType point = m_geometry->GetVertexAttributes(*vert).m_position;
			ExportPoint(out_stream, point);
		}

		auto face_end_iter = m_geometry->FacesEnd();
		for (auto face = m_geometry->FacesBegin(); face != face_end_iter; ++face)
		{
			std::vector<IndexType> face_indices;

			for (auto vert = m_geometry->FaceVertexBegin(*face); vert.IsValid(); ++vert)
			{
				face_indices.push_back(*vert);
			}

			StartFace(out_stream, face_indices.size());

			for (auto &vert : face_indices)
			{
				FaceAddIndex(out_stream, vert);
			}

			EndFace(out_stream);
		}

		EndGeometry(out_stream);

        */
		return true;
	}

protected:
	virtual void StartGeometry(std::ostream &out_stream, uint32_t num_points, uint32_t num_faces) = 0;
	virtual void ExportPoint(std::ostream &out_stream, const PositionType &position) = 0;
	virtual void StartFace(std::ostream &out_stream, const size_t &num_points) = 0;
	virtual void FaceAddIndex(std::ostream &out_stream, const IndexType &index) = 0;
	virtual void EndFace(std::ostream &out_stream) = 0;
	virtual void EndGeometry(std::ostream &out_stream) = 0;

	GeometryPtr m_geometry;
};  // class GeometryExporter

/**
 * Class to export geometries in the Obj format.
 */
template<class G>
class ObjExporter : public GeometryExporter<G>
{
public:
	/**
	 * Constructs the GeometryExporter with the given Geometry.
	 */
	explicit ObjExporter(typename GeometryExporter<G>::GeometryPtr geom) : GeometryExporter<G>(geom) {}

protected:
	void StartGeometry(std::ostream &out_stream, uint32_t num_points, uint32_t num_faces) final {}

	void ExportPoint(std::ostream &out_stream, const typename GeometryExporter<G>::PositionType &position) final
	{
		out_stream << "v " << position[0] << " " << position[1] << " " << position[2] << "\n";
	}

	void StartFace(std::ostream &out_stream, const size_t &num_points) final { out_stream << "f "; }

	void FaceAddIndex(std::ostream &out_stream, const typename GeometryExporter<G>::IndexType &index) final
	{
		out_stream << (index + 1) << " ";
	}

	void EndFace(std::ostream &out_stream) final { out_stream << "\n"; }

	void EndGeometry(std::ostream &out_stream) final {}

};  // class ObjExporter

/**
 * Exports the Geometry in Selector's binary format.
 *
 * Format Specification:
 *
 * | Header   |
 * | Vertex 0 |
 * |    ...   |
 * | Vertex n |
 * | Face 0   |
 * |    ...   |
 * | Face n   |
 */
template<class G>
class SelBinaryGeometryExporter : public GeometryExporter<G>
{
public:
	SelBinaryGeometryExporter(typename GeometryExporter<G>::GeometryPtr geom) : GeometryExporter<G>(geom) {}

protected:
	struct Header
	{
		char m_formatVersion;
		uint32_t m_numPoints;
		uint32_t m_numFaces;
	};  // struct Header

	void StartGeometry(std::ostream &out_stream, uint32_t num_points, uint32_t num_faces) final {}

	void ExportPoint(std::ostream &out_stream, const typename GeometryExporter<G>::PositionType &position) final {}

	void StartFace(std::ostream &out_stream, const size_t &num_points) final {}

	void FaceAddIndex(std::ostream &out_stream, const typename GeometryExporter<G>::IndexType &index) final {}

	void EndFace(std::ostream &out_stream) final {}

	void EndGeometry(std::ostream &out_stream) final {}

};  // class SelBinaryGeometryExporter

}  // namespace selector
#endif
