#include "tile.h"

#include "tile_layout.h"

#include "horizontal_tile_layout.h"
#include "vertical_tile_layout.h"
#include "window.h"

#include <imgui.h>

#include <boost/qvm/vec_access.hpp>

using namespace pagoda::math;

namespace pgeditor::gui
{
Tile::Tile() : m_attachedWindow{std::make_shared<Window>()} {}

Tile::Tile(const Vec2U& pos, const Vec2U& size)
  : m_size{size}, m_position{pos}, m_attachedWindow{std::make_shared<Window>()}, m_splitDirection{SplitDirection::None}
{
}

const Vec2U& Tile::GetSize() const { return m_size; }

const uint32_t Tile::GetWidth() const { return X(m_size); }

const uint32_t Tile::GetHeight() const { return Y(m_size); }

const pagoda::math::Vec2U& Tile::GetPosition() const { return m_position; }

Tile::SplitDirection Tile::GetSplitDirection() const { return m_splitDirection; }

void Tile::SetPosition(const pagoda::math::Vec2U& position)
{
	m_position = position;
	if (m_layout != nullptr) {
		m_layout->Layout(m_children);
	}
}

void Tile::SetSize(uint32_t width, uint32_t height) { SetSize({width, height}); }
void Tile::SetSize(const pagoda::math::Vec2U& size)
{
	m_size = size;
	if (m_layout != nullptr) {
		m_layout->Layout(m_children);
	}
}

void Tile::SetWidth(uint32_t width) { SetSize(width, Y(m_size)); }

void Tile::SetHeight(uint32_t height) { SetSize(X(m_size), height); }

void Tile::SetLeft(uint32_t left) { SetPosition({left, Y(m_position)}); }

void Tile::Split(SplitDirection direction)
{
	m_splitDirection = direction;
	switch (m_splitDirection) {
		case SplitDirection::Horizontal:
			m_layout = std::make_unique<HorizontalTileLayout>(shared_from_this());
			break;
		case SplitDirection::Vertical:
			m_layout = std::make_unique<VerticalTileLayout>(shared_from_this());
			break;
		case SplitDirection::None:
		case SplitDirection::Stack:
			break;
	}

	auto child1 = std::make_shared<Tile>(m_position, m_size);
	child1->m_parentTile = shared_from_this();
	auto child2 = std::make_shared<Tile>(m_position, m_size);
	child2->m_parentTile = shared_from_this();

	m_children.push_back(child1);
	m_children.push_back(child2);

	if (m_layout != nullptr) {
		m_layout->Layout(m_children);
	}
}

std::shared_ptr<Tile> Tile::GetParent() const
{
	if (m_parentTile.expired()) {
		return nullptr;
	}
	return m_parentTile.lock();
}

const std::vector<std::shared_ptr<Tile>>& Tile::GetChildren() const { return m_children; }

bool Tile::IsLeafTile() const { return m_children.empty(); }

void Tile::AttachWindow(std::shared_ptr<Window> window)
{
	if (m_attachedWindow != nullptr) {
		m_attachedWindow->SetParentTile(nullptr);
	}

	m_attachedWindow = window;

	if (m_attachedWindow != nullptr) {
		m_attachedWindow->SetParentTile(shared_from_this());
	}

	if (m_attachedWindow == nullptr) {
		m_attachedWindow = std::make_shared<Window>();
	}
}

void Tile::DetachWindow() { AttachWindow(nullptr); }

std::shared_ptr<Window> Tile::GetAttachedWindow() const { return m_attachedWindow; }

void Tile::Render()
{
	if (IsLeafTile() && m_attachedWindow != nullptr) {
		m_attachedWindow->SetParentTile(shared_from_this());
		m_attachedWindow->Render();
		return;
	}

	for (auto c : m_children) {
		c->Render();
	}
}
}  // namespace pgeditor::gui
