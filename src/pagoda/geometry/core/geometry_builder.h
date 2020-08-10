#ifndef PAGODA_GEOMETRY_CORE_GEOMETRY_BUILDER_H
#define PAGODA_GEOMETRY_CORE_GEOMETRY_BUILDER_H

#include "geometry.h"

#include <pagoda/geometry/algorithms/face_normal.h>
#include <pagoda/geometry/algorithms/traverse.h>

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/math/vec_base.h>

#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

namespace pagoda::geometry::core
{
/**
 * Helper class to build a geometry.
 *
 * Has methods to add points and start faces in a geometry.
 */
template<class Geometry>
class GeometryBuilderT
{
private:
	/// The Geometry index type
	using Index_t = typename Geometry::Index_t;
	/// The Geometry position type.
	using PositionType = typename Geometry::PositionType;
	using GeometryBuilder = GeometryBuilderT<Geometry>;
	using VertexAttributes = typename Geometry::VertexAttributes;

	struct PointData
	{
		PointData() : m_position{0, 0, 0}, m_index(Geometry::s_invalidIndex) {}
		PointData(const PositionType &pos) : m_position(pos), m_index(Geometry::s_invalidIndex) {}
		PointData(const PointData &o) : m_position(o.m_position), m_attributes(o.m_attributes), m_index(o.m_index) {}
		PointData(const PositionType &pos, const VertexAttributes &v)
		    : m_position(pos), m_attributes(v), m_index(Geometry::s_invalidIndex)
		{
		}

		bool IsInitialized() const { return m_index != Geometry::s_invalidIndex; }

		PositionType m_position;
		VertexAttributes m_attributes;
		Index_t m_index;
	};

public:
	/**
	 * Helper class to build faces in a geometry.
	 * Allows to add point indices to the face incrementally.
	 */
	class FaceBuilder
	{
	public:
		/// The Geometry index type
		using Index_t = typename Geometry::Index_t;

		/**
		 * Constructs a FaceBuilder with all the parameters.
		 */
		FaceBuilder(GeometryBuilder *builder, std::shared_ptr<Geometry> &geom, uint32_t numPoints = 0)
		    : m_builder(builder), m_geometry(geom)
		{
			LOG_TRACE(GeometryCore, "Created FaceBuilder with " << numPoints << " points");
			if (numPoints > 0)
			{
				m_faceIndices.reserve(numPoints);
			}
		}

		/**
		 * Adds a point index to the face.
		 * Points indices are added contiguously.
		 */
		void AddIndex(const Index_t &index)
		{
			LOG_TRACE(GeometryCore, "Adding index " << index << " to face builder.");
			DBG_ASSERT_MSG(m_faceIndices.size() <= m_faceIndices.capacity(),
			               "Trying to add a vertex index past the end");

			m_faceIndices.push_back(index);
		}

		/**
		 * Returns the i-th index in this face.
		 * This function does not check for out-of-bounds accesses.
		 */
		Index_t GetIndex(uint32_t i)
		{
			CRITICAL_ASSERT_MSG(i < m_faceIndices.size(), "Trying to get an index that hasn't been defined yet.");

			return m_faceIndices[i];
		}

		Index_t CloseFace()
		{
			START_PROFILE;
			DBG_ASSERT_MSG(m_faceIndices.size() >= 3, "Trying to close a face with less than 3 points");

			LOG_TRACE(GeometryCore, "Closing face");

			std::vector<typename Geometry::PointHandle> points;
			points.reserve(m_faceIndices.size());
			for (const auto &i : m_faceIndices)
			{
				points.emplace_back(m_builder->m_pointData[i].m_index);
			}

			auto face = m_geometry->CreateFace(points);
			std::size_t index = 0;
			algorithms::EachPointAroundFace(m_geometry.get(), face.m_face,
			                                [this, &index](Geometry *g, const typename Geometry::PointHandle &p) {
				                                auto &pointData = m_builder->m_pointData[m_faceIndices[index++]];
				                                pointData.m_index = p.GetIndex();
				                                g->GetVertexAttributes(pointData.m_index) = pointData.m_attributes;
				                                g->SetPosition(pointData.m_index, pointData.m_position);
			                                });

			auto normal = algorithms::face_normal(m_geometry, face.m_face);
			m_geometry->GetFaceAttributes(face.m_face).m_normal = normal;
			algorithms::EachPointAroundFace(m_geometry.get(), face.m_face,
			                                [normal](Geometry *g, const typename Geometry::PointHandle &p) {
				                                g->GetVertexAttributes(p).m_normal = normal;
			                                });

			return face.m_face;
		}

		GeometryBuilder *m_builder;
		/// The geometry.
		std::shared_ptr<Geometry> m_geometry;
		/// The indices for this face.
		std::vector<Index_t> m_faceIndices;
	};  // class FaceBuilder

	/**
	 * Creates a geometry builder for the given geometry.
	 */
	explicit GeometryBuilderT(std::shared_ptr<Geometry> geom) : m_geometry(geom) {}

	/**
	 * Starts a face in the geometry with the given number of points.
	 */
	FaceBuilder StartFace(uint32_t num_points = 0)
	{
		START_PROFILE;

		FaceBuilder face_builder(this, m_geometry, num_points);

		return face_builder;
	}

	/**
	 * Creates num_faces faces in the geometry all with num_points points.
	 */
	std::vector<FaceBuilder> StartFaces(uint32_t num_faces, uint32_t num_points = 0)
	{
		START_PROFILE;

		std::vector<FaceBuilder> face_builders;

		for (auto i = 0u; i < num_faces; ++i)
		{
			face_builders.emplace_back(this, m_geometry, num_points);
		}

		return face_builders;
	}

	/**
	 * Creates a series of faces in the geometry with the number of points specified in sizes.
	 */
	template<class T>
	std::vector<FaceBuilder> StartFaces(const T &sizes)
	{
		START_PROFILE;

		DBG_ASSERT_MSG(sizes.size() > 0, "Can't start faces without at least a size");

		std::vector<FaceBuilder> face_builders;

		for (auto size : sizes)
		{
			face_builders.emplace_back(this, m_geometry, size);
		}

		return face_builders;
	}

	/**
	 * Adds a point to the geometry and returns its index.
	 */
	Index_t AddPoint(const PositionType &pos)
	{
		START_PROFILE;
		Index_t newIndex = m_pointData.size();
		m_pointData.emplace(newIndex, PointData(pos));
		return newIndex;
	}

	/**
	 * Adds a point to the geometry, sets its data and returns its index.
	 */
	Index_t AddPoint(const PositionType &pos, const VertexAttributes &v)
	{
		START_PROFILE;
		Index_t newIndex = m_pointData.size();
		m_pointData.emplace(newIndex, PointData(pos, v));
		return newIndex;
	}

	/**
	 * Returns the geometry that is being built.
	 */
	std::shared_ptr<Geometry> GetGeometry() { return m_geometry; }

private:
	/// The Geometry to build.
	std::shared_ptr<Geometry> m_geometry;
	std::unordered_map<Index_t, PointData> m_pointData;
};  // class GeometryBuilderT

using GeometryBuilder = geometry::core::GeometryBuilderT<Geometry>;
using GeometryBuilderPtr = std::shared_ptr<GeometryBuilder>;

}  // namespace pagoda::geometry::core

#endif
