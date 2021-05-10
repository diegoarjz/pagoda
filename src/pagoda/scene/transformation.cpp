#include "transformation.h"
#include "boost/qvm/gen/vec_operations4.hpp"

#include <pagoda/common/instrument/profiler.h>

#include <boost/qvm/map_mat_mat.hpp>
#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/swizzle.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_mat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

using namespace pagoda::math;
using namespace boost;

namespace pagoda::scene
{
// clang-format off
const Vec3F Transformation::rightVector{-1, 0, 0};
const Vec3F Transformation::upVector   { 0, 1, 0};
const Vec3F Transformation::frontVector{ 0, 0, 1};
// clang-format on

Transformation::Transformation()
  : m_rotation(qvm::diag_mat(Vec3F{1, 1, 1})), m_translation{0, 0, 0}, m_scale{1, 1, 1}, m_transformMatrixIsDirty(true)
{
}

Transformation::Transformation(const Vec3F &rotEulerAngles, const Vec3F &translation, const Vec3F &scale)
  : m_rotation(qvm::rot_mat_xyz<3, float>(X(rotEulerAngles), Y(rotEulerAngles), Z(rotEulerAngles))),
    m_translation(translation), m_scale(scale), m_transformMatrixIsDirty(true)
{
}

Transformation::Transformation(const qvm::mat<float, 3, 3> &rotation, const Vec3F &translation, const Vec3F &scale)
  : m_rotation(rotation), m_translation(translation), m_scale(scale), m_transformMatrixIsDirty(true)
{
}

Transformation::Transformation(const Transformation &transform)
  : m_rotation(transform.m_rotation), m_translation(transform.m_translation), m_scale(transform.m_scale),
    m_transformMatrix(transform.m_transformMatrix), m_transformMatrixIsDirty(transform.m_transformMatrixIsDirty)
{
}

const Mat4x4F &Transformation::GetTransformationMatrix()
{
	START_PROFILE;
	if (m_transformMatrixIsDirty) {
		Mat4x4F rotMat;
		qvm::row<0>(rotMat) = XYZ0(qvm::row<0>(m_rotation));
		qvm::row<1>(rotMat) = XYZ0(qvm::row<1>(m_rotation));
		qvm::row<2>(rotMat) = XYZ0(qvm::row<2>(m_rotation));
		qvm::row<3>(rotMat) = Vec4F{0, 0, 0, 1};
		m_transformMatrix = qvm::translation_mat(m_translation) * rotMat * qvm::diag_mat(XYZ1(m_scale));
		m_transformMatrixIsDirty = false;
	}

	return m_transformMatrix;
}

void Transformation::SetPosition(const float &x, const float &y, const float &z)
{
	m_translation = Vec3F{x, y, z};
	m_transformMatrixIsDirty = true;
}

void Transformation::SetPosition(const Vec3F &pos)
{
	m_translation = pos;
	m_transformMatrixIsDirty = true;
}

const Vec3F &Transformation::GetPosition() const { return m_translation; }

void Transformation::Translate(const float &x, const float &y, const float &z)
{
	m_translation = m_translation + Vec3F{x, y, z};
	m_transformMatrixIsDirty = true;
}

void Transformation::Translate(const Vec3F &translation)
{
	m_translation = m_translation + translation;
	m_transformMatrixIsDirty = true;
}

void Transformation::SetScale(const float &scale)
{
	m_scale = Vec3F{scale, scale, scale};
	m_transformMatrixIsDirty = true;
}
void Transformation::SetScale(const float &x, const float &y, const float &z)
{
	m_scale = Vec3F{x, y, z};
	m_transformMatrixIsDirty = true;
}
void Transformation::SetScale(const Vec3F &s)
{
	m_scale = s;
	m_transformMatrixIsDirty = true;
}

const Vec3F &Transformation::GetScale() const { return m_scale; }

void Transformation::Yaw(const Radians<float> &angle)
{
	START_PROFILE;
	qvm::rotate_y(m_rotation, static_cast<float>(angle));
	m_transformMatrixIsDirty = true;
}
void Transformation::Pitch(const Radians<float> &angle)
{
	START_PROFILE;
	qvm::rotate_x(m_rotation, static_cast<float>(angle));
	m_transformMatrixIsDirty = true;
}
void Transformation::Roll(const Radians<float> &angle)
{
	START_PROFILE;
	qvm::rotate_z(m_rotation, static_cast<float>(angle));
	m_transformMatrixIsDirty = true;
}

void Transformation::Rotate(const Mat3x3F &r)
{
	START_PROFILE;
	m_rotation *= r;
	m_transformMatrixIsDirty = true;
}

const qvm::mat<float, 3, 3> &Transformation::GetRotation() const { return m_rotation; }

Transformation Transformation::operator*(const Transformation &t) const
{
	START_PROFILE;
	qvm::mat<float, 3, 3> scaleMat = qvm::diag_mat(m_scale);
	return Transformation(t.m_rotation * m_rotation, m_rotation * (scaleMat * t.m_translation) + m_translation,
	                      Vec3F{X(m_scale) * X(t.m_scale), Y(m_scale) * Y(t.m_scale), Z(m_scale) * Z(m_scale)});
}

bool Transformation::operator==(const Transformation &t) const
{
	return m_rotation == t.m_rotation && m_translation == t.m_translation && m_scale == t.m_scale;
}

bool Transformation::operator!=(const Transformation &t) const { return !(*this == t); }

Transformation Transformation::GetInverse() const
{
	START_PROFILE;
	qvm::mat<float, 3, 3> rot = qvm::transposed(m_rotation);
	return Transformation(rot, -m_translation, Vec3F{1 / X(m_scale), 1 / Y(m_scale), 1 / Z(m_scale)});
}

Vec3F Transformation::ConvertLocalToWorldCoordinates(const Vec3F &point)
{
	START_PROFILE;
	auto t = GetTransformationMatrix() * Vec4F{X(point), Y(point), Z(point), 1.0f};
	auto invW = 1.0f / W(t);
	return Vec3F{invW * X(t), invW * Y(t), invW * Z(t)};
}

Vec3F Transformation::ConvertWorldToLocalCoordinates(const Vec3F &point)
{
	START_PROFILE;
	auto t = getInverseTransformationMatrix() * Vec4F{X(point), Y(point), Z(point), 1.0f};
	auto invW = 1.0f / W(t);
	return Vec3F{invW * X(t), invW * Y(t), invW * Z(t)};
}

Vec3F Transformation::ConvertFromTransformationCoordinates(const Vec3F &point, Transformation &t)
{
	START_PROFILE;
	return ConvertWorldToLocalCoordinates(t.ConvertLocalToWorldCoordinates(point));
}

Vec3F Transformation::GetFrontDirection() const { return qvm::normalized(m_rotation * frontVector); }

Vec3F Transformation::GetRightDirection() const { return qvm::normalized(m_rotation * rightVector); }

Vec3F Transformation::GetUpDirection() const { return qvm::normalized(m_rotation * upVector); }

Mat4x4F Transformation::getInverseTransformationMatrix() const
{
	START_PROFILE;
	Mat4x4F inverseRotation;
	qvm::row<0>(inverseRotation) = XYZ1(qvm::row<0>(qvm::transposed(m_rotation)));
	qvm::row<1>(inverseRotation) = XYZ1(qvm::row<1>(qvm::transposed(m_rotation)));
	qvm::row<2>(inverseRotation) = XYZ1(qvm::row<2>(qvm::transposed(m_rotation)));
	qvm::row<3>(inverseRotation) = Vec4F{0, 0, 0, 1};
	auto inverseScale = Vec3F{1.0f / X(m_scale), 1.0f / Y(m_scale), 1.0f / Z(m_scale)};
	auto inverseTranslation = -1 * m_translation;
	Mat4x4F invTransMat = qvm::translation_mat(inverseTranslation);
	return qvm::diag_mat(XYZ1(inverseScale)) * inverseRotation * invTransMat;
}
}  // namespace pagoda::scene
