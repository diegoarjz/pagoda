#include "camera_manager.h"

#include <pagoda/common/debug/logger.h>

#include "camera.h"
#include "lens.h"
#include "transformation.h"

#include <vector>

using namespace pagoda::math;

namespace pgeditor::scene
{
class CameraManager::Impl
{
public:
	std::shared_ptr<Camera> CreateCamera()
	{
		auto camera = std::make_shared<Camera>();
		Lens lens;
		lens.SetPerspective(MathUtils<float>::degrees_to_radians(45.0f), GetAspectRatio(), 0.1f, 100.0f);
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

	void SetViewportSize(int width, int height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		if (m_activeCamera != nullptr)
		{
			m_activeCamera->GetLens().SetPerspective(MathUtils<float>::degrees_to_radians(45.0f), GetAspectRatio(),
			                                         0.1f, 100.0f);
		}
	}

	float GetAspectRatio() const { return static_cast<float>(m_viewportWidth) / static_cast<float>(m_viewportHeight); }

private:
	std::vector<std::shared_ptr<Camera>> m_cameras;
	std::shared_ptr<Camera> m_activeCamera;

	int m_viewportWidth;
	int m_viewportHeight;
};

CameraManager::CameraManager() : m_implementation(std::make_unique<Impl>()) {}
CameraManager::~CameraManager() {}

std::shared_ptr<Camera> CameraManager::CreateCamera() { return m_implementation->CreateCamera(); }
void CameraManager::SetActiveCamera(std::shared_ptr<Camera> camera) { m_implementation->SetActiveCamera(camera); }
void CameraManager::Update(double dT) { m_implementation->Update(dT); }
void CameraManager::SetViewportSize(int width, int height) { m_implementation->SetViewportSize(width, height); }

}  // namespace pgeditor::scene
