#ifndef SELECTOR_GEOMETRY_CORE_GEOMETRY_H_
#define SELECTOR_GEOMETRY_CORE_GEOMETRY_H_

#include <common/assertions.h>
#include <common/logger.h>
#include <common/profiler.h>
#include <math_lib/vec_base.h>

#include <cstdint>

#include "indexed_container.h"

#include <memory>

namespace selector
{
/**
 * Default attributes for vertices.
 */
struct DefaultVertexAttributes
{
	using PositionType = Vec3F;

	/// Vertex position.
	Vec3F m_position;
	/// Normal at the point. May differ from the face normal.
	Vec3F m_normal;
};  // struct VertAttributes

/**
 * Default attributes for edges.
 */
struct DefaultEdgeAttributes
{
};

/**
 * Default attributes for faces.
 */
struct DefaultFaceAttributes
{
	/// Face normal.
	Vec3F m_normal;
};

/**
 * Calculates the normal of a face.
 */
template<class Geometry>
inline typename Geometry::PositionType face_normal(std::shared_ptr<Geometry> geometry,
                                                   const typename Geometry::IndexType &face)
{
	START_PROFILE;
	DBG_ASSERT_MSG(geometry->FaceVertexSize(face) >= 3,
	               "Can't calculate the normal of a face with less than 3 vertices. Invalid Face?");
	auto f_v_circ = geometry->FaceVertexBegin(face);
	typename Geometry::PositionType pos0 = geometry->GetVertexAttributes(*f_v_circ).m_position;
	++f_v_circ;
	typename Geometry::PositionType pos1 = geometry->GetVertexAttributes(*f_v_circ).m_position;
	++f_v_circ;
	typename Geometry::PositionType pos2 = geometry->GetVertexAttributes(*f_v_circ).m_position;
	return normalized(cross_product(pos2 - pos1, pos0 - pos1));
}

template<class Topology, class F = DefaultFaceAttributes, class E = DefaultEdgeAttributes,
         class V = DefaultVertexAttributes>
class GeometryT
{
public:
	using IndexType = typename Topology::IndexType;
	using FacesIterator = typename Topology::FacesIterator;
	using EdgesIterator = typename Topology::EdgesIterator;
	using VertexIterator = typename Topology::VertexIterator;
	using FaceEdgeCirculator = typename Topology::FaceEdgeCirculator;
	using FaceVertexCirculator = typename Topology::FaceVertexCirculator;
	using FaceAttributes = F;
	using EdgeAttributes = E;
	using VertexAttributes = V;
	using PositionType = typename VertexAttributes::PositionType;

	static constexpr IndexType sInvalidIndex = std::numeric_limits<IndexType>::max();

	GeometryT(uint32_t num_vertices, uint32_t num_edges, uint32_t num_faces)
	{
		LOG_TRACE(GeometryCore, "New Geometry. #verts: %d #edges, %d #faces: %d", num_vertices, num_edges, num_faces);

		m_topology.ReserveFaces(num_faces);
		m_topology.ReserveEdges(num_edges);
		m_topology.ReserveVertices(num_vertices);

		m_vertexAttributes.reserve(num_vertices);
		m_edgeAttributes.reserve(num_edges);
		m_faceAttributes.reserve(num_faces);
	}

	IndexType CreateVertex(const PositionType &position)
	{
		START_PROFILE;
		LOG_TRACE(GeometryCore, "Creating a vertex: (%f, %f, %f)", position[0], position[1], position[2]);

		if (GetNumVertices() == GetReservedVertices())
		{
			LOG_TRACE(GeometryCore, "\tVertices are full: %d reserved vertices", GetReservedVertices());
			return sInvalidIndex;
		}

		IndexType vertex_index = m_topology.CreateVertex();
		IndexType attributes_index = m_vertexAttributes.Create();
		auto &attributes = m_vertexAttributes.Get(attributes_index);
		attributes.m_position = position;
		DBG_ASSERT(vertex_index == attributes_index);
		return vertex_index;
	}

	template<class T>
	IndexType CreateFace(const T &vertex_indices)
	{
		START_PROFILE;
		LOG_TRACE(GeometryCore, "Creating a face. Index count: %d", vertex_indices.size());

		if (GetNumFaces() == GetReservedFaces())
		{
			LOG_TRACE(GeometryCore, "\tFaces are full: %d reserved faces", GetReservedFaces());
			return sInvalidIndex;
		}

#ifdef DEBUG
		for (const auto &i : vertex_indices)
		{
			LOG_TRACE(GeometryCore, "\tIndex: %d", i);
		}
#endif

		auto face_index = m_topology.CreateFace(vertex_indices);
		auto attributes_index = m_faceAttributes.Create();
		DBG_ASSERT(face_index == attributes_index);

#ifdef DEBUG
		auto iter = FaceEdgeBegin(face_index);
#endif

		auto num_vertices = vertex_indices.size();
		for (uint32_t i = 0; i < num_vertices; ++i)
		{
#ifdef DEBUG
			auto edge_index = m_edgeAttributes.Create();
			DBG_ASSERT(edge_index == *iter);
			++iter;
#else
			m_edgeAttributes.Create();
#endif
		}
		return face_index;
	}

	VertexAttributes &GetVertexAttributes(const IndexType &vertex) { return m_vertexAttributes.Get(vertex); }
	EdgeAttributes &GetEdgeAttributes(const IndexType &edge) { return m_edgeAttributes.Get(edge); }
	FaceAttributes GetFaceAttributes(const IndexType &face) { return m_faceAttributes.Get(face); }

	FacesIterator FacesBegin() { return m_topology.FacesBegin(); }
	FacesIterator FacesEnd() { return m_topology.FacesEnd(); }
	EdgesIterator EdgesBegin() { return m_topology.EdgesBegin(); }
	EdgesIterator EdgesEnd() { return m_topology.EdgesEnd(); }
	VertexIterator VerticesBegin() { return m_topology.VerticesBegin(); }
	VertexIterator VerticesEnd() { return m_topology.VerticesEnd(); }
	FaceEdgeCirculator FaceEdgeBegin(const IndexType &face_index) { return m_topology.FaceEdgeBegin(face_index); }
	FaceVertexCirculator FaceVertexBegin(const IndexType &face_index) { return m_topology.FaceVertexBegin(face_index); }

	uint32_t FaceVertexSize(const IndexType &face_index) const { return m_topology.FaceVertexSize(face_index); }

	uint32_t GetNumFaces() const { return m_topology.GetNumFaces(); }
	uint32_t GetNumEdges() const { return m_topology.GetNumEdges(); }
	uint32_t GetNumVertices() const { return m_topology.GetNumVertices(); }

	uint32_t GetReservedFaces() const { return m_topology.GetReservedFaces(); }
	uint32_t GetReservedEdges() const { return m_topology.GetReservedEdges(); }
	uint32_t GetReservedVertices() const { return m_topology.GetReservedVertices(); }

private:
	Topology m_topology;
	IndexedContainer<IndexType, VertexAttributes> m_vertexAttributes;
	IndexedContainer<IndexType, EdgeAttributes> m_edgeAttributes;
	IndexedContainer<IndexType, FaceAttributes> m_faceAttributes;
};  // class Geometry

}  // namespace selector
#endif
