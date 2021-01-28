#include "window.h"

#include "tile.h"

#include <imgui.h>

namespace pgeditor::gui
{
std::size_t Window::nextWindowId = 0;

Window::Window()
{
	std::stringstream ss;
	ss << "window" << (nextWindowId++);
	m_windowName = ss.str();
}
Window::Window(std::shared_ptr<Tile> parentTile) : m_parentTile{parentTile} {}
Window::~Window() {}

void Window::SetParentTile(std::shared_ptr<Tile> tile) { m_parentTile = tile; }
std::shared_ptr<Tile> Window::GetTitle() const { return m_parentTile.lock(); }

const pagoda::math::Vec2U& Window::GetSize() const { return m_parentTile.lock()->GetSize(); }
const pagoda::math::Vec2U& Window::GetPosition() const { return m_parentTile.lock()->GetPosition(); }

void Window::Render()
{
	static const auto windowFlags =
	  ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

	ImGui::SetNextWindowSize(ImVec2(X(GetSize()), Y(GetSize())));
	ImGui::SetNextWindowPos(ImVec2(X(GetPosition()), Y(GetPosition())));

	ImGui::Begin(m_windowName.c_str(), nullptr, windowFlags);

	ImGui::End();
}

}  // namespace pgeditor::gui
