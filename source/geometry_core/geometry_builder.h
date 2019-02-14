#ifndef SELECTOR_GEOMETRY_CORE_GEOMETRY_BUILDER_H
#define SELECTOR_GEOMETRY_CORE_GEOMETRY_BUILDER_H

#include "common/assertions.h"
#include "common/profiler.h"

#include <memory>
#include <vector>
#include <cstdint>

namespace selector
{
/**
 * Helper class to build faces in a geometry.
 * Allows to add point indices to the face incrementally.
 */
template<class Geometry>
class FaceBuilder
{
public:
	/// The Geometry index type
	using IndexType = typename Geometry::IndexType;

	/**
	 * Constructs a FaceBuilder with all the parameters.
	 */
	FaceBuilder(std::shared_ptr<Geometry> &geom, uint32_t num_points) : m_geometry(geom)
	{
		m_faceIndices.reserve(num_points);
	}

	/**
	 * Adds a point index to the face.
	 * Points indices are added contiguously.
	 */
	void AddIndex(const IndexType &index)
	{
		DBG_ASSERT_MSG(m_faceIndices.size() < m_faceIndices.capacity(), "Trying to add a vertex index past the end");

		if (m_faceIndices.size() >= m_faceIndices.capacity())
		{
			return;
		}

		m_faceIndices.push_back(index);
	}

	/**
	 * Returns the i-th index in this face.
	 * This function does not check for out-of-bounds accesses.
	 */
	IndexType GetIndex(uint32_t i)
	{
		CRITICAL_ASSERT_MSG(i < m_faceIndices.size(), "Trying to get an index that hasn't been defined yet.");

		return m_faceIndices[i];
	}

	IndexType CloseFace() { return m_geometry->CreateFace(m_faceIndices); }

	/// The geometry.
	std::shared_ptr<Geometry> m_geometry;
	/// The indices for this face.
	std::vector<IndexType> m_faceIndices;
};  // class FaceBuilder

/**
 * Helper class to build a geometry.
 *
 * Has methods to add points and start faces in a geometry.
 * Points and faces are allocated contiguously in the geometry.
 */
template<class Geometry>
class GeometryBuilderT
{
private:
	/// The Geometry index type
	using IndexType = typename Geometry::IndexType;
	/// The Geometry position type.
	using PositionType = typename Geometry::PositionType;

public:
	/**
	 * Creates a geometry builder for the given geometry.
	 */
	explicit GeometryBuilderT(std::shared_ptr<Geometry> geom) : m_geometry(geom), m_createdFaceCount(0) {}

	/**
	 * Starts a face in the geometry with the given number of points.
	 * Faces will be created contiguously.
	 */
	FaceBuilder<Geometry> StartFace(uint32_t num_points)
	{
		START_PROFILE;

		DBG_ASSERT_MSG(num_points > 0, "A face must have more than 0 points");
		DBG_ASSERT_MSG(m_createdFaceCount < m_geometry->GetReservedFaces(), "Trying to create more faces than allowed");

		FaceBuilder<Geometry> face_builder(m_geometry, num_points);
		++m_createdFaceCount;

		return face_builder;
	}

	/**
	 * Creates num_faces faces in the geometry all with num_points points.
	 * Each face is created contiguously to the one before.
	 */
	std::vector<FaceBuilder<Geometry>> StartFaces(uint32_t num_faces, uint32_t num_points)
	{
		START_PROFILE;

		DBG_ASSERT_MSG(num_points > 0, "A face must have more than 0 points.");
		DBG_ASSERT_MSG((m_createdFaceCount + num_faces) <= m_geometry->GetReservedFaces(),
		               "Trying to create more faces than allowed");

		std::vector<FaceBuilder<Geometry>> face_builders;

		for (auto i = 0u; i < num_faces; ++i)
		{
			face_builders.emplace_back(m_geometry, num_points);
			++m_createdFaceCount;
		}

		return face_builders;
	}

	/**
	 * Creates a series of faces in the geometry with the number of points specified in sizes.
	 */
	template<class T>
	std::vector<FaceBuilder<Geometry>> StartFaces(const T &sizes)
	{
		START_PROFILE;

		DBG_ASSERT_MSG(sizes.size() > 0, "Can't start faces without at least a size");
		DBG_ASSERT_MSG((m_createdFaceCount + sizes.size()) <= m_geometry->GetReservedFaces(),
		               "Trying to create more faces than allowed");

		std::vector<FaceBuilder<Geometry>> face_builders;

		for (auto size : sizes)
		{
			face_builders.emplace_back(m_geometry, size);
			++m_createdFaceCount;
		}

		return face_builders;
	}

	/**
	 * Adds a point to the geometry and returns its index.
	 * Points are added sequentially to the geometry.
	 */
	IndexType AddPoint(const PositionType &pos)
	{
		START_PROFILE;

		IndexType vert_index = m_geometry->CreateVertex(pos);
		return vert_index;
	}

	/**
	 * Returns the geometry that is being built.
	 */
	std::shared_ptr<Geometry> GetGeometry() { return m_geometry; }

private:
	/// The Geometry to build.
	std::shared_ptr<Geometry> m_geometry;
	uint32_t m_createdFaceCount;
};  // class GeometryBuilderT

}  // namespace selector

#endif
