#pragma once

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/radians.h>
#include <pagoda/math/vec_base.h>

namespace pagoda::scene
{
/**
 * Represents a \c Transformation from a local coordinate system into the
 * world coordinate system.
 */
class Transformation
{
	public:
	static const math::Vec3F upVector;
	static const math::Vec3F frontVector;
	static const math::Vec3F rightVector;

	/**
	 * Creates an identity \c Transformation.
	 */
	Transformation();

	/**
	 * Creates a \c Transformation with rotation, translation and scale.
	 * @param rotEulerAngles    Euler angles in radians.
	 * @param translation       Translation.
	 * @param scale             Scale.
	 */
	Transformation(const math::Vec3F &rotEulerAngles, const math::Vec3F &translation, const math::Vec3F &scale);

	/**
	 * Creates a \c Transformation with a rotation, translation and scale.
	 * @param rotation          Rotation matrix.
	 * @param translation       Translation.
	 * @param scale             Scale.
	 */
	Transformation(const math::Mat3x3F &rotation, const math::Vec3F &translation, const math::Vec3F &scale);

	/**
	 * Creates a \c Transformation by copying \p transform.
	 */
	Transformation(const Transformation &transform);

	/**
	 * Returns the \c math::Mat4x4F for this transformation.
	 */
	const math::Mat4x4F &GetTransformationMatrix();

	/**
	 * Sets the origin of the local coordinate system to (\p x, \p y, \p z).
	 */
	void SetPosition(const float &x, const float &y, const float &z);

	/**
	 * Sets the origin of the local coordinate system to \p pos.
	 */
	void SetPosition(const math::Vec3F &pos);

	/**
	 * Returns the origin of the local coordinate system.
	 */
	const math::Vec3F &GetPosition() const;

	/**
	 * Translates the origin of the local coordinate system.
	 */
	void Translate(const float &x, const float &y, const float &z);

	/**
	 * Translates the origin of the local coordinate system.
	 */
	void Translate(const math::Vec3F &translation);

	/**
	 * Sets the scale of the local coordinate system to uniformly scale
	 * along all axis by \p scale.
	 */
	void SetScale(const float &scale);

	/**
	 * Sets the scale of the local coordinate system.
	 */
	void SetScale(const float &x, const float &y, const float &z);

	/**
	 * Sets the scale of the local coordinate system.
	 */
	void SetScale(const math::Vec3F &s);

	/**
	 * Gets the scale of the local coordinate system.
	 */
	const math::Vec3F &GetScale() const;

	/**
	 * Rotates the local coordinate system around its Y Axis.
	 */
	void Yaw(const pagoda::math::Radians<float> &angle);

	/**
	 * Rotates the local coordinate system around its X Axis.
	 */
	void Pitch(const pagoda::math::Radians<float> &angle);

	/**
	 * Rotates the local coordinate system around its Z Axis.
	 */
	void Roll(const pagoda::math::Radians<float> &angle);

	/**
	 * Rotates the local coordinate system with the given matrix \p r.
	 */
	void Rotate(const math::Mat3x3F &r);

	/**
	 * Returns the rotation matrix between the local and world coordinate systems.
	 */
	const math::Mat3x3F &GetRotation() const;

	/**
	 * Creates a transformation that is equivalent of applying \p this
	 * transformation and then \p t.
	 */
	Transformation operator*(const Transformation &t) const;

	bool operator==(const Transformation &t) const;
	bool operator!=(const Transformation &t) const;

	/**
	 * Returns the inverse of this \c Transformation.
	 */
	Transformation GetInverse() const;

	/**
	 * Converts \p point, defined in the coordinate system fo this \c Transformation to
	 * world coordinates.
	 */
	math::Vec3F ConvertLocalToWorldCoordinates(const math::Vec3F &point);

	/**
	 * Converts \p point, defined in world coordinates to the coordinate system of this
	 * \c Transformation.
	 */
	math::Vec3F ConvertWorldToLocalCoordinates(const math::Vec3F &point);

	/**
	 * Converts \p point, defined in the coordinate system of \p t to the coordinate
	 * system of this \c Transformation.
	 */
	math::Vec3F ConvertFromTransformationCoordinates(const math::Vec3F &point, Transformation &t);

	math::Vec3F GetFrontDirection() const;
	math::Vec3F GetRightDirection() const;
	math::Vec3F GetUpDirection() const;

	private:
	math::Mat4x4F getInverseTransformationMatrix() const;

	math::Mat3x3F m_rotation;
	math::Vec3F m_translation;
	math::Vec3F m_scale;
	math::Mat4x4F m_transformMatrix;
	bool m_transformMatrixIsDirty;
};

}  // namespace pagoda::scene
