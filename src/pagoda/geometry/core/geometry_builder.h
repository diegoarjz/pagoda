#ifndef PAGODA_GEOMETRY_CORE_GEOMETRY_BUILDER_H
#define PAGODA_GEOMETRY_CORE_GEOMETRY_BUILDER_H

#include "indexed_container.h"

#include <pagoda/common/debug/assertions.h>
#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <pagoda/math/vec_base.h>

#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <cstdint>
#include <memory>
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
		PointData(const PointData &o) : m_position(o.m_position), m_index(o.m_index) {}
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

			auto pd0 = m_builder->m_pointData.Get(m_faceIndices[0]);
			auto pd1 = m_builder->m_pointData.Get(m_faceIndices[1]);
			auto pd2 = m_builder->m_pointData.Get(m_faceIndices[2]);

			boost::qvm::vec<float, 3> normal{0, 0, 0};
			for (auto i = 0u; i < m_faceIndices.size(); ++i)
			{
				auto nextIndex = (i + 1) % m_faceIndices.size();
				auto curr = m_builder->m_pointData.Get(m_faceIndices[i]);
				auto next = m_builder->m_pointData.Get(m_faceIndices[nextIndex]);

				// Newell's Method
				X(normal) += (Y(curr.m_position) - Y(next.m_position)) * (Z(curr.m_position) + Z(next.m_position));
				Y(normal) += (Z(curr.m_position) - Z(next.m_position)) * (X(curr.m_position) + X(next.m_position));
				Z(normal) += (X(curr.m_position) - X(next.m_position)) * (Y(curr.m_position) + Y(next.m_position));
			}
			DBG_ASSERT_MSG(boost::qvm::mag_sqr(normal) > 0,
			               "The normal's magnitude is 0. Are all face points collinear?");
			boost::qvm::normalize(normal);

			auto face = m_geometry->CreateFace(pd0.m_index, pd1.m_index, pd2.m_index);
			m_geometry->GetFaceAttributes(face.m_face).m_normal = normal;
			for (auto i = 0u; i < 3; ++i)
			{
				auto &pointData = m_builder->m_pointData.Get(m_faceIndices[i]);
				pointData.m_index = m_geometry->GetPoint(face.m_splitPoints[i]);
				m_geometry->GetVertexAttributes(pointData.m_index) = pointData.m_attributes;
				m_geometry->SetPosition(m_geometry->GetPoint(face.m_splitPoints[i]),
				                        m_builder->m_pointData.Get(m_faceIndices[i]).m_position);
			}

			auto currentEdge = m_geometry->GetOutEdge(face.m_splitPoints[2]);

			for (auto i = 3u; i < m_faceIndices.size(); ++i)
			{
				LOG_TRACE(GeometryCore, " Going to split edge " << currentEdge);

				auto &pointData = m_builder->m_pointData.Get(m_faceIndices[i]);
				if (pointData.m_index == Geometry::s_invalidIndex)
				{
					LOG_TRACE(GeometryCore, " Needs to create a new point");
					auto newSplitPoint = m_geometry->SplitEdge(currentEdge);
					pointData.m_index = m_geometry->GetPoint(newSplitPoint);
					m_geometry->GetVertexAttributes(pointData.m_index) = pointData.m_attributes;
				}
				else
				{
					LOG_TRACE(GeometryCore, " Reusing previous point " << pointData.m_index);
					m_geometry->SplitEdge(currentEdge, pointData.m_index);
				}
				m_geometry->SetPosition(pointData.m_index, pointData.m_position);
				currentEdge = m_geometry->GetNextEdge(currentEdge);
			}

			for (auto fpIter = m_geometry->FacePointCirculatorBegin(face.m_face); fpIter.IsValid(); ++fpIter)
			{
				m_geometry->GetVertexAttributes(*fpIter).m_normal = normal;
			}

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
		return m_pointData.Create(PointData(pos));
	}

	/**
	 * Adds a point to the geometry, sets its data and returns its index.
	 */
	Index_t AddPoint(const PositionType &pos, const VertexAttributes &v)
	{
		START_PROFILE;
		return m_pointData.Create(PointData(pos, v));
	}

	/**
	 * Returns the geometry that is being built.
	 */
	std::shared_ptr<Geometry> GetGeometry() { return m_geometry; }

private:
	/// The Geometry to build.
	std::shared_ptr<Geometry> m_geometry;
	AssociativeIndexedContainer<Index_t, PointData> m_pointData;
};  // class GeometryBuilderT

}  // namespace pagoda::geometry::core

#endif
