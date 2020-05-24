#include "camera.h"

#include <pagoda/common/instrument/profiler.h>

#include "attachable_visitor.h"
#include "boost/qvm/gen/vec_operations2.hpp"
#include "boost/qvm/gen/vec_operations4.hpp"
#include "transformation.h"

#include <boost/qvm/map_mat_mat.hpp>
#include <boost/qvm/map_mat_vec.hpp>
#include <boost/qvm/map_vec_mat.hpp>
#include <boost/qvm/mat_access.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/swizzle.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_mat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

using namespace pagoda::math;

using namespace boost;

namespace pgeditor::scene
{
Camera::Camera()
    : m_position{0, 0, 0},
      m_viewDirection{0, 0, -1},
      m_viewMatrix(qvm::identity_mat<float, 4>()),
      m_viewMatrixDirty(true)
{
}

void Camera::SetPosition(const boost::qvm::vec<float, 3> &pos)
{
	if (m_position != pos)
	{
		m_position = pos;
		m_viewMatrixDirty = true;
	}
}

void Camera::SetViewDirection(const boost::qvm::vec<float, 3> &dir)
{
	DBG_ASSERT(qvm::mag_sqr(dir) > 0);
	if (m_viewDirection != dir)
	{
		m_viewDirection = qvm::normalized(dir);
		m_viewMatrixDirty = true;
	}
}

void Camera::SetTransformation(const Transformation &t)
{
	SetPosition(t.GetPosition());
	SetViewDirection(t.GetFrontDirection());
}

void Camera::SetLens(const Lens &lens) { m_lens = lens; }
const Lens &Camera::GetLens() const { return m_lens; }

template<typename T>
qvm::mat<T, 4, 4> look_at(const qvm::vec<T, 3> &eye, const qvm::vec<T, 3> &target, const qvm::vec<T, 3> &upDirection)
{
	using namespace qvm;
	using mat4 = mat<T, 4, 4>;
	using vec3 = vec<T, 3>;
	using vec4 = vec<T, 4>;

	mat4 mat;

	vec3 fwd = normalized(eye - target);
	vec3 right = normalized(cross(upDirection, fwd));
	vec3 up = normalized(cross(fwd, right));

	row<0>(mat) = XYZ0(right);
	row<1>(mat) = XYZ0(up);
	row<2>(mat) = XYZ0(fwd);
	row<3>(mat) = vec4{0, 0, 0, 1};

	return mat * translation_mat(-eye);
}

const boost::qvm::vec<float, 3> &Camera::GetPosition() const { return m_position; }

const boost::qvm::vec<float, 3> &Camera::GetViewDirection() const { return m_viewDirection; }
boost::qvm::vec<float, 3> Camera::GetRightVector() const
{
	return normalized(qvm::cross(Transformation::upVector, GetViewDirection()));
}
boost::qvm::vec<float, 3> Camera::GetUpVector() const
{
	return normalized(qvm::cross(GetViewDirection(), GetRightVector()));
}

const boost::qvm::mat<float, 4, 4> &Camera::GetViewMatrix()
{
	START_PROFILE;

	if (m_viewMatrixDirty)
	{
		boost::qvm::vec<float, 3> target = m_position + m_viewDirection;
		m_viewMatrix = look_at(m_position, target, Transformation::upVector);
		m_viewMatrixDirty = false;
	}
	return m_viewMatrix;
}

const boost::qvm::mat<float, 4, 4> &Camera::GetProjectionMatrix() { return m_lens.GetProjectionMatrix(); }

void Camera::AcceptVisitor(IAttachableVisitor *visitor) { visitor->Visit(this); }
}  // namespace pgeditor::scene
