#pragma once

#include "tile_layout.h"

#include <pagoda/math/vec_base.h>

#include <memory>
#include <vector>

namespace pgeditor::gui
{
class Window;
class TileLayout;

class Tile : public std::enable_shared_from_this<Tile>
{
	public:
	Tile();

	/**
	 * Constructs a \c Tile.
	 */
	Tile(const pagoda::math::Vec2U& pos, const pagoda::math::Vec2U& size);

	const pagoda::math::Vec2U& GetPosition() const;
	void SetPosition(const pagoda::math::Vec2U& position);
	void SetLeft(uint32_t left);

	const pagoda::math::Vec2U& GetSize() const;
	void SetSize(const pagoda::math::Vec2U& size);
	void SetSize(uint32_t width, uint32_t height);
	void SetWidth(uint32_t width);
	void SetHeight(uint32_t height);
	const uint32_t GetWidth() const;
	const uint32_t GetHeight() const;

	enum class SplitDirection
	{
		None,
		Vertical,
		Horizontal,
		Stack
	};
	SplitDirection GetSplitDirection() const;

	void Split(SplitDirection direction);
	std::shared_ptr<Tile> GetParent() const;
	const std::vector<std::shared_ptr<Tile>>& GetChildren() const;

	bool IsLeafTile() const;

	void AttachWindow(std::shared_ptr<Window> window);
	void DetachWindow();
	std::shared_ptr<Window> GetAttachedWindow() const;

	void Render();

	private:
	pagoda::math::Vec2U m_size;
	pagoda::math::Vec2U m_position;
	std::weak_ptr<Tile> m_parentTile;
	std::vector<std::shared_ptr<Tile>> m_children;
	std::shared_ptr<Window> m_attachedWindow;

	SplitDirection m_splitDirection;
	std::unique_ptr<TileLayout> m_layout;
};
}  // namespace pgeditor::gui
