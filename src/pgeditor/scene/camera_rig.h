#pragma once

#include <pagoda/math/vec_base.h>

#include <memory>

namespace pgeditor::scene
{
class Node;

class CameraRig final
{
public:
	CameraRig(std::shared_ptr<Node> cameraNode);

	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void Rotate(const float &vertical, const float &horizontal);

	void Update(float dT);

private:
	std::shared_ptr<Node> m_cameraNode;
	boost::qvm::vec<float, 3> m_cameraMovement;
	float m_verticalRotation;
	float m_horizontalRotation;
};
}  // namespace pgeditor::scene
