#ifndef SELECTOR_GEOMETRY_CORE_SCOPE_H_
#define SELECTOR_GEOMETRY_CORE_SCOPE_H_

#include <math_lib/matrix_base.h>
#include <math_lib/plane.h>
#include <math_lib/vec_base.h>

#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/swizzle.hpp>
#include <boost/qvm/vec_operations.hpp>

#include <array>
#include <memory>

namespace selector
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
	Scope(const Vec3F &pos, const Vec3F &size, const Mat3x3F &rot);
	Scope(const std::array<Vec3F, 8> &boxPoints);

	Vec3F GetPosition() const;
	void SetPosition(const Vec3F &pos);
	Vec3F GetSize() const;
	void SetSize(const Vec3F &size);
	Mat3x3F GetRotation() const;
	void SetRotation(const Mat3x3F &rotation);
	Mat3x3F GetInverseRotation() const;

	Vec3F GetXAxis() const;
	Vec3F GetYAxis() const;
	Vec3F GetZAxis() const;
	Vec3F GetAxis(const std::string &axisName) const;
	Vec3F GetAxis(char axisName) const;

	Plane<float> GetXYPlane() const;
	Plane<float> GetXZPlane() const;
	Plane<float> GetYZPlane() const;

	Vec3F LocalPointInWorld(const Vec3F &localPoint) const;
	Vec3F GetLocalPoint(const BoxPoints &p) const;
	Vec3F GetWorldPoint(const BoxPoints &p) const;
	std::array<Vec3F, 8> GetWorldPoints() const;
	Vec3F GetLocalVector(const Vec3F &worldVector) const;
	Vec3F GetWorldVector(const Vec3F &localVector) const;

	Vec3F GetCenterPointInWorld() const;
	Vec3F GetCenterPointInLocal() const;

	template<class Geometry>
	static Scope FromGeometryAndConstrainedRotation(const std::shared_ptr<Geometry> geom, const Mat3x3F &rotation)
	{
		Scope s;
		auto pIter = geom->PointsBegin();
		auto p0 = geom->GetPosition(*pIter);
		s.SetRotation(rotation);
		auto xAxis = Vec3F(boost::qvm::col<0>(rotation));
		auto yAxis = Vec3F(boost::qvm::col<1>(rotation));
		auto zAxis = Vec3F(boost::qvm::col<2>(rotation));

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
		Vec3F size{maxX - minX, maxY - minY, maxZ - minZ};
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

		Mat3x3F rotation;
		boost::qvm::col<0>(rotation) = xAxis;
		boost::qvm::col<1>(rotation) = yAxis;
		boost::qvm::col<2>(rotation) = zAxis;

		return Scope::FromGeometryAndConstrainedRotation(geom, rotation);
	}

private:
	/// Scope position in world coordinates
	Vec3F m_position;
	/// Scope size in world coordinates
	Vec3F m_size;
	/// Scope rotation
	Mat3x3F m_rotation;
};  // class Scope
}  // namespace selector
#endif
