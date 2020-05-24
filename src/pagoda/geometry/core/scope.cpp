#include "scope.h"

#include <boost/qvm/map_mat_mat.hpp>
#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/vec_operations.hpp>

using namespace pagoda::math;

namespace pagoda::geometry::core
{
Scope::Scope() : m_position{0, 0, 0}, m_size{0, 0, 0}, m_rotation(boost::qvm::diag_mat(boost::qvm::vec<float, 3>{1, 1, 1})) {}

Scope::Scope(const boost::qvm::vec<float, 3> &pos, const boost::qvm::vec<float, 3> &size, const boost::qvm::mat<float, 3, 3> &rot) : m_position(pos), m_size(size), m_rotation(rot)
{
}

Scope::Scope(const std::array<boost::qvm::vec<float, 3>, 8> &boxPoints)
{
	auto origin = boxPoints[static_cast<int>(BoxPoints::LowerBottomLeft)];
	auto lbr = boxPoints[static_cast<int>(BoxPoints::LowerBottomRight)];
	auto ltl = boxPoints[static_cast<int>(BoxPoints::LowerTopLeft)];
	auto hbl = boxPoints[static_cast<int>(BoxPoints::HigherBottomLeft)];
	auto xAxis = lbr - origin;
	auto yAxis = ltl - origin;
	auto zAxis = boost::qvm::cross(xAxis, yAxis);

	m_position = origin;
	m_size = boost::qvm::vec<float, 3>{boost::qvm::mag(xAxis), boost::qvm::mag(yAxis), boost::qvm::mag(origin - hbl)};
	boost::qvm::col<0>(m_rotation) = boost::qvm::normalized(xAxis);
	boost::qvm::col<1>(m_rotation) = boost::qvm::normalized(yAxis);
	boost::qvm::col<2>(m_rotation) = boost::qvm::normalized(zAxis);
}

boost::qvm::vec<float, 3> Scope::GetPosition() const { return m_position; }

void Scope::SetPosition(const boost::qvm::vec<float, 3> &pos) { m_position = pos; }

boost::qvm::vec<float, 3> Scope::GetSize() const { return m_size; }

void Scope::SetSize(const boost::qvm::vec<float, 3> &size) { m_size = size; }

boost::qvm::mat<float, 3, 3> Scope::GetRotation() const { return m_rotation; }

void Scope::SetRotation(const boost::qvm::mat<float, 3, 3> &rotation) { m_rotation = rotation; }

boost::qvm::mat<float, 3, 3> Scope::GetInverseRotation() const { return boost::qvm::transposed(m_rotation); }

boost::qvm::vec<float, 3> Scope::GetXAxis() const { return boost::qvm::col<0>(m_rotation); }
boost::qvm::vec<float, 3> Scope::GetYAxis() const { return boost::qvm::col<1>(m_rotation); }
boost::qvm::vec<float, 3> Scope::GetZAxis() const { return boost::qvm::col<2>(m_rotation); }

boost::qvm::vec<float, 3> Scope::GetAxis(const std::string &axisName) const
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

boost::qvm::vec<float, 3> Scope::GetAxis(char axisName) const
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
	return boost::qvm::vec<float, 3>();
}

Plane<float> Scope::GetXYPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetZAxis()); }
Plane<float> Scope::GetXZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetYAxis()); }
Plane<float> Scope::GetYZPlane() const { return Plane<float>::FromPointAndNormal(m_position, GetXAxis()); }

boost::qvm::vec<float, 3> Scope::LocalPointInWorld(const boost::qvm::vec<float, 3> &localPoint) const
{
	return m_position + GetXAxis() * X(localPoint) + GetYAxis() * Y(localPoint) + GetZAxis() * Z(localPoint);
}

boost::qvm::vec<float, 3> Scope::GetLocalPoint(const BoxPoints &p) const
{
	uint32_t index = static_cast<uint32_t>(p);
	boost::qvm::vec<float, 3> localPoint;
	X(localPoint) = ((index & (1 << 0)) != 0 ? X(m_size) : 0);
	Y(localPoint) = ((index & (1 << 1)) != 0 ? Y(m_size) : 0);
	Z(localPoint) = ((index & (1 << 2)) != 0 ? Z(m_size) : 0);
	return localPoint;
}

boost::qvm::vec<float, 3> Scope::GetWorldPoint(const BoxPoints &p) const { return LocalPointInWorld(GetLocalPoint(p)); }

std::array<boost::qvm::vec<float, 3>, 8> Scope::GetWorldPoints() const
{
	std::array<boost::qvm::vec<float, 3>, 8> boxPoints;
	for (int i = 0; i < 8; ++i)
	{
		boxPoints[i] = GetWorldPoint(static_cast<BoxPoints>(i));
	}
	return boxPoints;
}

boost::qvm::vec<float, 3> Scope::GetLocalVector(const boost::qvm::vec<float, 3> &worldVector) const
{
	return boost::qvm::vec<float, 3>{boost::qvm::dot(GetXAxis(), worldVector), boost::qvm::dot(GetYAxis(), worldVector),
	             boost::qvm::dot(GetZAxis(), worldVector)};
}

boost::qvm::vec<float, 3> Scope::GetWorldVector(const boost::qvm::vec<float, 3> &localVector) const
{
	return GetXAxis() * X(localVector) + GetYAxis() * Y(localVector) + GetZAxis() * Z(localVector);
}

boost::qvm::vec<float, 3> Scope::GetCenterPointInWorld() const { return LocalPointInWorld(GetCenterPointInLocal()); }

boost::qvm::vec<float, 3> Scope::GetCenterPointInLocal() const
{
	return 0.5f * (GetLocalPoint(BoxPoints::LowerBottomLeft) + GetLocalPoint(BoxPoints::HigherTopRight));
}
}  // namespace pagoda::geometry::core
