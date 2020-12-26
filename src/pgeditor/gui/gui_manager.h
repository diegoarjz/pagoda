#pragma once

#include <pgeditor/common/manager.h>

#include <memory>

namespace pgeditor::gui
{
class GuiManager : public common::Manager
{
	public:
	GuiManager();
	~GuiManager() override;

	bool Init() override;
	void Update(double dT) override;
	void Destroy() override;

	private:
	class GuiManagerImpl;
	std::unique_ptr<GuiManagerImpl> m_implementation;
};
}  // namespace pgeditor::gui
