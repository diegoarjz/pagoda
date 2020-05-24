#pragma once

#include <memory>

namespace pgeditor::scene
{
class Camera;

class CameraManager final
{
public:
	CameraManager();
	virtual ~CameraManager();

	std::shared_ptr<Camera> CreateCamera();
	void SetActiveCamera(std::shared_ptr<Camera> camera);

	void Update(double dT);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pgeditor::scene
