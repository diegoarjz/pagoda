#include "scope.h"

#include "math_lib/transpose.h"

#include <boost/qvm/map_mat_mat.hpp>
#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace pagoda
{
Scope::Scope() : m_position{0, 0, 0}, m_size{0, 0, 0}, m_rotation(boost::qvm::diag_mat(Vec3F{1, 1, 1})) {}

Scope::Scope(const Vec3F &pos, const Vec3F &size, const Mat3x3F &rot) : m_position(pos), m_size(size), m_rotation(rot)
{
}

Scope::Scope(const std::array<Vec3F, 8> &boxPoints)
{
	auto origin = boxPoints[static_cast<int>(BoxPoints::LowerBottomLeft)];
	auto lbr = boxPoints[static_cast<int>(BoxPoints::LowerBottomRight)];
	auto ltl = boxPoints[static_cast<int>(BoxPoints::LowerTopLeft)];
	auto hbl = boxPoints[static_cast<int>(BoxPoints::HigherBottomLeft)];
	auto xAxis = lbr - origin;
	auto yAxis = ltl - origin;
	auto zAxis = boost::qvm::cross(xAxis, yAxis);

	m_position = origin;
	m_size = Vec3F{boost::qvm::mag(xAxis), boost::qvm::mag(yAxis), boost::qvm::mag(origin - hbl)};
	boost::qvm::col<0>(m_rotation) = boost::qvm::normalized(xAxis);
	boost::qvm::col<1>(m_rotation) = boost::qvm::normalized(yAxis);
	boost::qvm::col<2>(m_rotation) = boost::qvm::normalized(zAxis);
}

Vec3F Scope::GetPosition() const { return m_position; }

void Scope::SetPosition(const Vec3F &pos) { m_position = pos; }

Vec3F Scope::GetSize() const { return m_size; }

void Scope::SetSize(const Vec3F &size) { m_size = size; }

Mat3x3F Scope::GetRotation() const { return m_rotation; }

void Scope::SetRotation(const Mat3x3F &rotation) { m_rotation = rotation; }

Mat3x3F Scope::GetInverseRotation() const { return boost::qvm::transposed(m_rotation); }

Vec3F Scope::GetXAxis() const { return boost::qvm::col<0>(m_rotation); }
Vec3F Scope::GetYAxis() const { return boost::qvm::col<1>(m_rotation); }
Vec3F Scope::GetZAxis() const { return boost::qvm::col<2>(m_rotation); }

Vec3F Scope::GetAxis(const std::string &axisName) const
{
	if (axisName == "x")
	{
		return GetXAxis();
	}
	else if (axisName == "y")
	{
		return GetYAxis();
	}
	CRITICAL_ASSERT_MSG(axisName == "z", "Axis name must be one of x, y or z.");
	return GetZAxis();
}

Vec3F Scope::GetAxis(char axisName) const
{
	switch (axisName)
	{
		case 'x':
			return GetXAxis();
		case 'y':
			return GetYAxis();
		case 'z':
			return GetZAxis();
		default:
			CRITICAL_ASSERT_MSG(false, "Axis must be one of x, y or z.");
	}
	return Vec3F();
}

Plane<float> Scope::GetXYPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetZAxis()); }
Plane<float> Scope::GetXZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetYAxis()); }
Plane<float> Scope::GetYZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetXAxis()); }

Vec3F Scope::LocalPointInWorld(const Vec3F &localPoint) const
{
	return m_position + GetXAxis() * X(localPoint) + GetYAxis() * Y(localPoint) + GetZAxis() * Z(localPoint);
}

Vec3F Scope::GetLocalPoint(const BoxPoints &p) const
{
	uint32_t index = static_cast<uint32_t>(p);
	Vec3F localPoint;
	X(localPoint) = ((index & (1 << 0)) != 0 ? X(m_size) : 0);
	Y(localPoint) = ((index & (1 << 1)) != 0 ? Y(m_size) : 0);
	Z(localPoint) = ((index & (1 << 2)) != 0 ? Z(m_size) : 0);
	return localPoint;
}

Vec3F Scope::GetWorldPoint(const BoxPoints &p) const { return LocalPointInWorld(GetLocalPoint(p)); }

std::array<Vec3F, 8> Scope::GetWorldPoints() const
{
	std::array<Vec3F, 8> boxPoints;
	for (int i = 0; i < 8; ++i)
	{
		boxPoints[i] = GetWorldPoint(static_cast<BoxPoints>(i));
	}
	return boxPoints;
}

Vec3F Scope::GetLocalVector(const Vec3F &worldVector) const
{
	return Vec3F{boost::qvm::dot(GetXAxis(), worldVector), boost::qvm::dot(GetYAxis(), worldVector),
	             boost::qvm::dot(GetZAxis(), worldVector)};
}

Vec3F Scope::GetWorldVector(const Vec3F &localVector) const
{
	return GetXAxis() * X(localVector) + GetYAxis() * Y(localVector) + GetZAxis() * Z(localVector);
}

Vec3F Scope::GetCenterPointInWorld() const { return LocalPointInWorld(GetCenterPointInLocal()); }

Vec3F Scope::GetCenterPointInLocal() const
{
	return 0.5f * (GetLocalPoint(BoxPoints::LowerBottomLeft) + GetLocalPoint(BoxPoints::HigherTopRight));
}
}  // namespace pagoda
