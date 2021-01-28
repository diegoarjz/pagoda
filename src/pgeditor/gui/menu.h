#pragma once

#include <functional>
#include <string>
#include <vector>

namespace pgeditor::gui
{
class Menu
{
	public:
	struct MenuItem
	{
		std::string m_label;
		std::function<void()> m_action;

		MenuItem(const std::string& label);
		MenuItem(const std::string& label, const std::function<void()> action);
	};

	Menu(const std::string& label);
	Menu(const std::string& label, const std::vector<MenuItem>&& menuItems);

	const std::string& GetLabel() const;

	void AddMenuItem(MenuItem&& item);
	const std::vector<MenuItem>& GetMenuItems() const;

	private:
	std::string m_label;
	std::vector<MenuItem> m_menuItems;
};
}  // namespace pgeditor::gui
