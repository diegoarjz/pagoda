#include "scope.h"

#include "math_lib/cross_product.h"
#include "math_lib/length.h"
#include "math_lib/normalize.h"

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

Vec3F Scope::GetXAxis() const { return m_rotation.Col(1); }
Vec3F Scope::GetYAxis() const { return m_rotation.Col(2); }
Vec3F Scope::GetZAxis() const { return m_rotation.Col(3); }

Plane<float> Scope::GetXYPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetZAxis()); }
Plane<float> Scope::GetXZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetYAxis()); }
Plane<float> Scope::GetYZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetXAxis()); }
}  // namespace selector
