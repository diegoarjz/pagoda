#include "camera_rig.h"

#include <pagoda/common/debug/logger.h>

#include <boost/qvm/gen/mat_operations2.hpp>
#include <boost/qvm/map_mat_mat.hpp>
#include <boost/qvm/mat_operations.hpp>
#include <boost/qvm/vec_mat_operations.hpp>
#include <boost/qvm/vec_operations.hpp>

#include "boost/qvm/gen/vec_operations3.hpp"
#include "node.h"

using namespace pagoda::math;
using namespace boost;

namespace pgeditor::scene
{
CameraRig::CameraRig(std::shared_ptr<Node> cameraNode)
    : m_cameraNode(cameraNode), m_cameraMovement{0, 0, 0}, m_verticalRotation(0.0f), m_horizontalRotation(0.0f)
{
}

void CameraRig::MoveForward() { m_cameraMovement += m_cameraNode->GetWorldTransformation().GetFrontDirection(); }

void CameraRig::MoveBackward() { m_cameraMovement -= m_cameraNode->GetWorldTransformation().GetFrontDirection(); }

void CameraRig::MoveLeft() { m_cameraMovement -= m_cameraNode->GetWorldTransformation().GetRightDirection(); }

void CameraRig::MoveRight() { m_cameraMovement += m_cameraNode->GetWorldTransformation().GetRightDirection(); }

void CameraRig::MoveUp() { m_cameraMovement += Transformation::upVector; }

void CameraRig::MoveDown() { m_cameraMovement -= Transformation::upVector; }

void CameraRig::Rotate(const float &vertical, const float &horizontal)
{
	m_verticalRotation = vertical;
	m_horizontalRotation = horizontal;
}

void CameraRig::Update(float dT)
{
	if (qvm::mag_sqr(m_cameraMovement) > 0)
	{
		qvm::normalize(m_cameraMovement);
		m_cameraNode->Translate(10.0f * dT * m_cameraMovement);
		m_cameraMovement = boost::qvm::vec<float, 3>{0.0f, 0.0f, 0.0f};
	}
	if (m_verticalRotation != 0.0f)
	{
		m_cameraNode->Pitch(Radians<float>(0.1 * m_verticalRotation * dT));
		m_verticalRotation = 0.0f;
	}
	if (m_horizontalRotation != 0.0f)
	{
		auto rotAxis = qvm::normalized(qvm::transposed(m_cameraNode->GetWorldTransformation().GetRotation()) *
		                               Transformation::upVector);
		qvm::mat<float, 3, 3> rot = qvm::rot_mat<3>(rotAxis, 0.1 * m_horizontalRotation * dT);

		m_cameraNode->Rotate(rot);
		m_horizontalRotation = 0.0f;
	}
}
}  // namespace pgeditor::scene
