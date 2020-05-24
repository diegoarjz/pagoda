#include "camera_manager.h"

#include "camera.h"
#include "lens.h"
#include "transformation.h"

#include <vector>

namespace pgeditor::scene
{
class CameraManager::Impl
{
public:
	std::shared_ptr<Camera> CreateCamera()
	{
		auto camera = std::make_shared<Camera>();
		Lens lens;
		lens.SetPerspective(pagoda::math::MathUtils<float>::degrees_to_radians(45.0f), 1.0f, 0.1f, 100.0f);
		camera->SetLens(lens);
		m_cameras.push_back(camera);
		if (m_activeCamera == nullptr)
		{
			SetActiveCamera(camera);
		}
		return camera;
	}

	void SetActiveCamera(std::shared_ptr<Camera> camera) { m_activeCamera = camera; }

	void Update(double dT) {}

private:
	std::vector<std::shared_ptr<Camera>> m_cameras;
	std::shared_ptr<Camera> m_activeCamera;
};

CameraManager::CameraManager() : m_implementation(std::make_unique<Impl>()) {}
CameraManager::~CameraManager() {}

std::shared_ptr<Camera> CameraManager::CreateCamera() { return m_implementation->CreateCamera(); }
void CameraManager::SetActiveCamera(std::shared_ptr<Camera> camera) { m_implementation->SetActiveCamera(camera); }
void CameraManager::Update(double dT) { m_implementation->Update(dT); }

}  // namespace pgeditor::scene
