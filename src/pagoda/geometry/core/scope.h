#ifndef PAGODA_GEOMETRY_CORE_SCOPE_H_
#define PAGODA_GEOMETRY_CORE_SCOPE_H_

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/plane.h>
#include <pagoda/math/vec_base.h>

#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/swizzle.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <array>
#include <memory>

namespace pagoda::geometry::core
{
/**
 * Represents an oriented bounding box to be used as a local frame of coordinates for geometries.
 */
class Scope
{
public:
	/**
	 * Names of the points in the box defined by this scope.
	 *
	 * \verbatim
	 *       g-------h
	 *      /|      /|
	 *     / |     / |
	 *    e--|----f  |
	 *    |  c----|--d
	 *    | /     | /
	 *    a-------b
	 *    a - LowerBottomLeft (0)
	 *    b - LowerBottomRight (1)
	 *    c - LowerTopLeft (2)
	 *    d - LowerTopRight (3)
	 *    e - HigherBottomLeft (4)
	 *    f - HigherBottomRight (5)
	 *    g - HigherTopLeft (6)
	 *    h - HigherTopRight (7)
	 * \endverbatim
	 */
	enum class BoxPoints
	{
		// Binary Representation
		// zyx
		LowerBottomLeft = 0,   // Origin
		LowerBottomRight = 1,  // Along X Axis
		LowerTopLeft = 2,      // Along Y Axis
		LowerTopRight = 3,

		HigherBottomLeft = 4,  // Along Z Axis
		HigherBottomRight = 5,
		HigherTopLeft = 6,
		HigherTopRight = 7,
	};

	Scope();
	Scope(const boost::qvm::vec<float, 3> &pos, const boost::qvm::vec<float, 3> &size, const boost::qvm::mat<float, 3, 3> &rot);
	Scope(const std::array<boost::qvm::vec<float, 3>, 8> &boxPoints);

	boost::qvm::vec<float, 3> GetPosition() const;
	void SetPosition(const boost::qvm::vec<float, 3> &pos);
	boost::qvm::vec<float, 3> GetSize() const;
	void SetSize(const boost::qvm::vec<float, 3> &size);
	boost::qvm::mat<float, 3, 3> GetRotation() const;
	void SetRotation(const boost::qvm::mat<float, 3, 3> &rotation);
	boost::qvm::mat<float, 3, 3> GetInverseRotation() const;

	boost::qvm::vec<float, 3> GetXAxis() const;
	boost::qvm::vec<float, 3> GetYAxis() const;
	boost::qvm::vec<float, 3> GetZAxis() const;
	boost::qvm::vec<float, 3> GetAxis(const std::string &axisName) const;
	boost::qvm::vec<float, 3> GetAxis(char axisName) const;

	math::Plane<float> GetXYPlane() const;
	math::Plane<float> GetXZPlane() const;
	math::Plane<float> GetYZPlane() const;

	boost::qvm::vec<float, 3> LocalPointInWorld(const boost::qvm::vec<float, 3> &localPoint) const;
	boost::qvm::vec<float, 3> GetLocalPoint(const BoxPoints &p) const;
	boost::qvm::vec<float, 3> GetWorldPoint(const BoxPoints &p) const;
	std::array<boost::qvm::vec<float, 3>, 8> GetWorldPoints() const;
	boost::qvm::vec<float, 3> GetLocalVector(const boost::qvm::vec<float, 3> &worldVector) const;
	boost::qvm::vec<float, 3> GetWorldVector(const boost::qvm::vec<float, 3> &localVector) const;

	boost::qvm::vec<float, 3> GetCenterPointInWorld() const;
	boost::qvm::vec<float, 3> GetCenterPointInLocal() const;

	template<class Geometry>
	static Scope FromGeometryAndConstrainedRotation(const std::shared_ptr<Geometry> geom, const boost::qvm::mat<float, 3, 3> &rotation)
	{
		Scope s;
		auto pIter = geom->PointsBegin();
		auto p0 = geom->GetPosition(*pIter);
		s.SetRotation(rotation);
		auto xAxis = boost::qvm::vec<float, 3>(boost::qvm::col<0>(rotation));
		auto yAxis = boost::qvm::vec<float, 3>(boost::qvm::col<1>(rotation));
		auto zAxis = boost::qvm::vec<float, 3>(boost::qvm::col<2>(rotation));

		float minX = std::numeric_limits<float>::max();
		float maxX = -std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxY = -std::numeric_limits<float>::max();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = -std::numeric_limits<float>::max();

		auto pIterEnd = geom->PointsEnd();
		for (/**/; pIter != pIterEnd; ++pIter)
		{
			auto diff = geom->GetPosition(*pIter) - p0;
			float xProjection = boost::qvm::dot(xAxis, diff);
			float yProjection = boost::qvm::dot(yAxis, diff);
			float zProjection = boost::qvm::dot(zAxis, diff);

			minX = std::min(minX, xProjection);
			maxX = std::max(maxX, xProjection);
			minY = std::min(minY, yProjection);
			maxY = std::max(maxY, yProjection);
			minZ = std::min(minZ, zProjection);
			maxZ = std::max(maxZ, zProjection);
		}
		boost::qvm::vec<float, 3> size{maxX - minX, maxY - minY, maxZ - minZ};
		s.SetPosition(p0 + xAxis * minX + yAxis * minY + zAxis * minZ);
		s.SetSize(size);

		return s;
	}

	template<class Geometry>
	static Scope FromGeometry(const std::shared_ptr<Geometry> geom)
	{
		auto pIter = geom->PointsBegin();
		auto p0 = geom->GetPosition(*pIter);
		pIter++;
		auto p1 = geom->GetPosition(*pIter);
		pIter++;
		auto p2 = geom->GetPosition(*pIter);
		pIter++;

		auto xAxis = normalized(p1 - p0);
		auto yAxis = p2 - p1;
		auto zAxis = boost::qvm::normalized(boost::qvm::cross(xAxis, yAxis));
		yAxis = boost::qvm::normalized(boost::qvm::cross(zAxis, xAxis));

		boost::qvm::mat<float, 3, 3> rotation;
		boost::qvm::col<0>(rotation) = xAxis;
		boost::qvm::col<1>(rotation) = yAxis;
		boost::qvm::col<2>(rotation) = zAxis;

		return Scope::FromGeometryAndConstrainedRotation(geom, rotation);
	}

private:
	/// Scope position in world coordinates
	boost::qvm::vec<float, 3> m_position;
	/// Scope size in world coordinates
	boost::qvm::vec<float, 3> m_size;
	/// Scope rotation
	boost::qvm::mat<float, 3, 3> m_rotation;
};  // class Scope
}  // namespace pagoda::geometry::core
#endif
