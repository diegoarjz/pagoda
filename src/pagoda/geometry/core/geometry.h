#ifndef PAGODA_GEOMETRY_CORE_GEOMETRY_H_
#define PAGODA_GEOMETRY_CORE_GEOMETRY_H_

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>
#include <pagoda/math/vec_base.h>

#include <cstdint>

#include "indexed_container.h"
#include "split_point_topology.h"

#include <boost/qvm/vec_operations.hpp>

#include <memory>

namespace pagoda::geometry::core
{
/**
 * Default attributes for vertices.
 */
struct DefaultVertexAttributes
{
	/// Normal at the point. May differ from the face normal.
	math::Vec3F m_normal;
	/// Texture coordinates.
	math::Vec2F m_texCoords;
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
	math::Vec3F m_normal;
};

/**
 * Calculates the normal of a face.
 */
template<class Geometry>
inline typename Geometry::PositionType face_normal(std::shared_ptr<Geometry> geometry,
                                                   const typename Geometry::Index_t &face)
{
	START_PROFILE;
	auto facePointCirc = geometry->FacePointCirculatorBegin(face);
	typename Geometry::PositionType pos0 = geometry->GetPosition(*facePointCirc);
	++facePointCirc;
	typename Geometry::PositionType pos1 = geometry->GetPosition(*facePointCirc);
	++facePointCirc;
	typename Geometry::PositionType pos2 = geometry->GetPosition(*facePointCirc);
	return boost::qvm::normalized(boost::qvm::cross(pos2 - pos1, pos0 - pos1));
}

template<class Topology = SplitPointTopology, class F = DefaultFaceAttributes, class E = DefaultEdgeAttributes,
         class V = DefaultVertexAttributes>
class GeometryBase : public Topology
{
public:
	using Index_t = typename Topology::Index_t;
	using FaceAttributes = F;
	using EdgeAttributes = E;
	using VertexAttributes = V;
	using PositionType = math::Vec3F;

	void SetPosition(const Index_t &index, const PositionType &p) { m_vertexPositions.GetOrCreate(index, p) = p; }

	PositionType &GetPosition(const Index_t &index) { return m_vertexPositions.GetOrCreate(index); }

	VertexAttributes &GetVertexAttributes(const Index_t &vertex) { return m_vertexAttributes.GetOrCreate(vertex); }
	EdgeAttributes &GetEdgeAttributes(const Index_t &edge) { return m_edgeAttributes.GetOrCreate(edge); }
	FaceAttributes &GetFaceAttributes(const Index_t &face) { return m_faceAttributes.GetOrCreate(face); }

private:
	AssociativeIndexedContainer<Index_t, PositionType> m_vertexPositions;
	AssociativeIndexedContainer<Index_t, VertexAttributes> m_vertexAttributes;
	AssociativeIndexedContainer<Index_t, EdgeAttributes> m_edgeAttributes;
	AssociativeIndexedContainer<Index_t, FaceAttributes> m_faceAttributes;
};  // class Geometry

}  // namespace pagoda::geometry::core
#endif
