#pragma once

#include "tile_layout.h"

#include <pagoda/math/vec_base.h>

#include <memory>
#include <vector>

namespace pgeditor::gui
{
class Tile;

class VerticalTileLayout : public TileLayout
{
	public:
	VerticalTileLayout(std::shared_ptr<Tile> parentTile);
	~VerticalTileLayout() override;

	void Layout(const std::vector<std::shared_ptr<Tile>>& tiles) override;
};
}  // namespace pgeditor::gui

