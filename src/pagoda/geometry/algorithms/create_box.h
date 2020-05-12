#ifndef PAGODA_GEOMETRY_OPERATIONS_CREATE_BOX_H_
#define PAGODA_GEOMETRY_OPERATIONS_CREATE_BOX_H_

#include <pagoda/common/debug/logger.h>
#include <pagoda/geometry/core/geometry.h>
#include <pagoda/geometry/core/geometry_builder.h>
#include <pagoda/geometry/core/scope.h>

namespace pagoda::geometry::algorithms
{
template<class G>
class CreateBox
{
private:
	using Geometry = G;
	using GeometryPtr = std::shared_ptr<Geometry>;

public:
	CreateBox(const float &xSize, const float &ySize, const float &zSize)
	{
		float halfXSize = 0.5f * xSize;
		float halfYSize = 0.5f * ySize;
		float halfZSize = 0.5f * zSize;

		m_points[0] = math::Vec3F{-halfXSize, -halfYSize, -halfZSize};
		m_points[1] = math::Vec3F{halfXSize, -halfYSize, -halfZSize};
		m_points[2] = math::Vec3F{-halfXSize, halfYSize, -halfZSize};
		m_points[3] = math::Vec3F{halfXSize, halfYSize, -halfZSize};

		m_points[4] = math::Vec3F{-halfXSize, -halfYSize, halfZSize};
		m_points[5] = math::Vec3F{halfXSize, -halfYSize, halfZSize};
		m_points[6] = math::Vec3F{-halfXSize, halfYSize, halfZSize};
		m_points[7] = math::Vec3F{halfXSize, halfYSize, halfZSize};
	}

	CreateBox(const std::array<math::Vec3F, 8> &points) : m_points(points) {}

	CreateBox(const core::Scope &s) : m_points(s.GetWorldPoints()) {}

	void Execute(GeometryPtr geometryOut)
	{
		START_PROFILE;
		LOG_TRACE(GeometryOperations, "CreateBox.");

		core::GeometryBuilderT<Geometry> builder(geometryOut);
		std::array<typename Geometry::Index_t, 8> pointIndices = {
		    builder.AddPoint(m_points[0]), builder.AddPoint(m_points[1]), builder.AddPoint(m_points[2]),
		    builder.AddPoint(m_points[3]), builder.AddPoint(m_points[4]), builder.AddPoint(m_points[5]),
		    builder.AddPoint(m_points[6]), builder.AddPoint(m_points[7]),
		};

		// Bottom face
		auto face = builder.StartFace(4);
		face.AddIndex(pointIndices[0]);
		face.AddIndex(pointIndices[2]);
		face.AddIndex(pointIndices[3]);
		face.AddIndex(pointIndices[1]);
		face.CloseFace();

		// Top Face
		face = builder.StartFace(4);
		face.AddIndex(pointIndices[4]);
		face.AddIndex(pointIndices[5]);
		face.AddIndex(pointIndices[7]);
		face.AddIndex(pointIndices[6]);
		face.CloseFace();

		// Front Face
		face = builder.StartFace(4);
		face.AddIndex(pointIndices[0]);
		face.AddIndex(pointIndices[1]);
		face.AddIndex(pointIndices[5]);
		face.AddIndex(pointIndices[4]);
		face.CloseFace();

		// Left Face
		face = builder.StartFace(4);
		face.AddIndex(pointIndices[0]);
		face.AddIndex(pointIndices[4]);
		face.AddIndex(pointIndices[6]);
		face.AddIndex(pointIndices[2]);
		face.CloseFace();

		// Right Face
		face = builder.StartFace(4);
		face.AddIndex(pointIndices[1]);
		face.AddIndex(pointIndices[3]);
		face.AddIndex(pointIndices[7]);
		face.AddIndex(pointIndices[5]);
		face.CloseFace();

		// Back Face
		face = builder.StartFace(4);
		face.AddIndex(pointIndices[2]);
		face.AddIndex(pointIndices[6]);
		face.AddIndex(pointIndices[7]);
		face.AddIndex(pointIndices[3]);
		face.CloseFace();
	}

private:
	std::array<math::Vec3F, 8> m_points;
};

}  // namespace pagoda::geometry::algorithms

#endif
