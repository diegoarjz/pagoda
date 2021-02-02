#pragma once

#include <memory>
#include <vector>

namespace pgeditor::gui
{
class Tile;

class TileLayout
{
	public:
	TileLayout(std::shared_ptr<Tile> parentTile);
	virtual ~TileLayout();

	virtual void Layout(const std::vector<std::shared_ptr<Tile>>& tiles) = 0;

	protected:
	std::shared_ptr<Tile> m_parentTile;
};
}  // namespace pgeditor::gui
