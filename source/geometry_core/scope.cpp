#include "scope.h"

#include "math_lib/cross_product.h"
#include "math_lib/length.h"
#include "math_lib/normalize.h"
#include "math_lib/transpose.h"

namespace selector
{
Scope::Scope() : m_position(0, 0, 0), m_size(0, 0, 0), m_rotation(1) {}

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
	auto zAxis = cross_product(xAxis, yAxis);

	m_position = origin;
	m_size = Vec3F(length(xAxis), length(yAxis), length(origin - hbl));
	m_rotation.SetCol(0, normalized(xAxis));
	m_rotation.SetCol(1, normalized(yAxis));
	m_rotation.SetCol(2, normalized(zAxis));
}

Vec3F Scope::GetPosition() const { return m_position; }

void Scope::SetPosition(const Vec3F &pos) { m_position = pos; }

Vec3F Scope::GetSize() const { return m_size; }

void Scope::SetSize(const Vec3F &size) { m_size = size; }

Mat3x3F Scope::GetRotation() const { return m_rotation; }

void Scope::SetRotation(const Mat3x3F &rotation) { m_rotation = rotation; }

Mat3x3F Scope::GetInverseRotation() const { return transposed(m_rotation); }

Vec3F Scope::GetXAxis() const { return m_rotation.Col(0); }
Vec3F Scope::GetYAxis() const { return m_rotation.Col(1); }
Vec3F Scope::GetZAxis() const { return m_rotation.Col(2); }

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
	CRITICAL_ASSERT_MSG(axisName == "z", "Axis name must be one of x, y or z");
	return GetZAxis();
}

Plane<float> Scope::GetXYPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetZAxis()); }
Plane<float> Scope::GetXZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetYAxis()); }
Plane<float> Scope::GetYZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetXAxis()); }

Vec3F Scope::LocalPointInWorld(const Vec3F &localPoint) const
{
	return m_position + GetXAxis() * localPoint[0] + GetYAxis() * localPoint[1] + GetZAxis() * localPoint[2];
}

Vec3F Scope::GetLocalPoint(const BoxPoints &p) const
{
	uint32_t index = static_cast<uint32_t>(p);
	Vec3F localPoint;
	localPoint[0] = ((index & (1 << 0)) != 0 ? m_size[0] : 0);
	localPoint[1] = ((index & (1 << 1)) != 0 ? m_size[1] : 0);
	localPoint[2] = ((index & (1 << 2)) != 0 ? m_size[2] : 0);
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
	return Vec3F(dot_product(GetXAxis(), worldVector), dot_product(GetYAxis(), worldVector),
	             dot_product(GetZAxis(), worldVector));
}

Vec3F Scope::GetWorldVector(const Vec3F &localVector) const
{
	return GetXAxis() * localVector.X() + GetYAxis() * localVector.Y() + GetZAxis() * localVector.Z();
}

Vec3F Scope::GetCenterPointInWorld() const { return LocalPointInWorld(GetCenterPointInLocal()); }

Vec3F Scope::GetCenterPointInLocal() const
{
	return 0.5f * (GetLocalPoint(BoxPoints::LowerBottomLeft) + GetLocalPoint(BoxPoints::HigherTopRight));
}
}  // namespace selector
