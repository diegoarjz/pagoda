#pragma once

#include <pagoda/math/matrix_base.h>
#include <pagoda/math/radians.h>
#include <pagoda/math/vec_base.h>

namespace pgeditor::scene
{
/**
 * Represents a \c Transformation from a local coordinate system into the
 * world coordinate system.
 */
class Transformation
{
public:
	static const boost::qvm::vec<float, 3> upVector;
	static const boost::qvm::vec<float, 3> frontVector;
	static const boost::qvm::vec<float, 3> rightVector;

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
	Transformation(const boost::qvm::vec<float, 3> &rotEulerAngles, const boost::qvm::vec<float, 3> &translation,
	               const boost::qvm::vec<float, 3> &scale);
	/**
	 * Creates a \c Transformation with a rotation, translation and scale.
	 * @param rotation          Rotation matrix.
	 * @param translation       Translation.
	 * @param scale             Scale.
	 */
	Transformation(const boost::qvm::mat<float, 3, 3> &rotation, const boost::qvm::vec<float, 3> &translation,
	               const boost::qvm::vec<float, 3> &scale);
	/**
	 * Creates a \c Transformation by copying \p transform.
	 */
	Transformation(const Transformation &transform);

	/**
	 * Returns the \c boost::qvm::mat<float, 4, 4> for this transformation.
	 */
	const boost::qvm::mat<float, 4, 4> &GetTransformationMatrix();

	/**
	 * Sets the origin of the local coordinate system to (\p x, \p y, \p z).
	 */
	void SetPosition(const float &x, const float &y, const float &z);
	/**
	 * Sets the origin of the local coordinate system to \p pos.
	 */
	void SetPosition(const boost::qvm::vec<float, 3> &pos);
	/**
	 * Returns the origin of the local coordinate system.
	 */
	const boost::qvm::vec<float, 3> &GetPosition() const;

	/**
	 * Translates the origin of the local coordinate system.
	 */
	void Translate(const float &x, const float &y, const float &z);
	/**
	 * Translates the origin of the local coordinate system.
	 */
	void Translate(const boost::qvm::vec<float, 3> &translation);

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
	void SetScale(const boost::qvm::vec<float, 3> &s);
	/**
	 * Gets the scale of the local coordinate system.
	 */
	const boost::qvm::vec<float, 3> &GetScale() const;

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
	void Rotate(const boost::qvm::mat<float, 3, 3> &r);

	/**
	 * Returns the rotation matrix between the local and world coordinate systems.
	 */
	const boost::qvm::mat<float, 3, 3> &GetRotation() const;

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
	boost::qvm::vec<float, 3> ConvertLocalToWorldCoordinates(const boost::qvm::vec<float, 3> &point);

	/**
	 * Converts \p point, defined in world coordinates to the coordinate system of this
	 * \c Transformation.
	 */
	boost::qvm::vec<float, 3> ConvertWorldToLocalCoordinates(const boost::qvm::vec<float, 3> &point);

	/**
	 * Converts \p point, defined in the coordinate system of \p t to the coordinate
	 * system of this \c Transformation.
	 */
	boost::qvm::vec<float, 3> ConvertFromTransformationCoordinates(const boost::qvm::vec<float, 3> &point,
	                                                               Transformation &t);

	boost::qvm::vec<float, 3> GetFrontDirection() const;
	boost::qvm::vec<float, 3> GetRightDirection() const;
	boost::qvm::vec<float, 3> GetUpDirection() const;

private:
	boost::qvm::mat<float, 4, 4> getInverseTransformationMatrix() const;

	boost::qvm::mat<float, 3, 3> m_rotation;
	boost::qvm::vec<float, 3> m_translation;
	boost::qvm::vec<float, 3> m_scale;
	boost::qvm::mat<float, 4, 4> m_transformMatrix;
	bool m_transformMatrixIsDirty;
};

}  // namespace pgeditor::scene
