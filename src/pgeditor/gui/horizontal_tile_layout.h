#pragma once

#include "tile_layout.h"

#include <pagoda/math/vec_base.h>

#include <memory>
#include <vector>

namespace pgeditor::gui
{
class Tile;

class HorizontalTileLayout : public TileLayout
{
	public:
	HorizontalTileLayout(std::shared_ptr<Tile> parentTile);
	~HorizontalTileLayout() override;

	void Layout(const std::vector<std::shared_ptr<Tile>>& tiles) override;
};
}  // namespace pgeditor::gui
