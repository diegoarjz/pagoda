#include "horizontal_tile_layout.h"

#include "tile.h"

using namespace pagoda;
using namespace pagoda::math;

namespace pgeditor::gui
{
HorizontalTileLayout::HorizontalTileLayout(std::shared_ptr<Tile> parentTile) : TileLayout(parentTile) {}
HorizontalTileLayout::~HorizontalTileLayout() {}

void HorizontalTileLayout::Layout(const std::vector<std::shared_ptr<Tile>>& tiles)
{
	auto parentPos = m_parentTile->GetPosition();
	auto parentSize = m_parentTile->GetSize();

	auto width = static_cast<float>(X(parentSize)) / tiles.size();
	auto height = Y(parentSize);
	auto currentPos = X(parentPos);
	for (auto& t : tiles) {
		t->SetSize({static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
		t->SetPosition({currentPos, Y(parentPos)});
		currentPos += width;
	}
}
}  // namespace pgeditor::gui
