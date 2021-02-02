#pragma once

#include <pgeditor/common/manager.h>

#include <memory>

namespace pgeditor::event
{
class EventManager;
}

namespace pgeditor::gui
{
class GuiManager : public common::Manager
{
	public:
	GuiManager(event::EventManager* eventManager);
	~GuiManager() override;

	bool Init() override;
	void Update(double dT) override;
	void Destroy() override;

	void SetWindowSize(uint32_t width, uint32_t height);

	private:
	class GuiManagerImpl;
	std::unique_ptr<GuiManagerImpl> m_implementation;
};
}  // namespace pgeditor::gui
