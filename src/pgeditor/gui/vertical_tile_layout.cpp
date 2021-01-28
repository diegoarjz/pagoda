#include "vertical_tile_layout.h"

#include "tile.h"

using namespace pagoda;
using namespace pagoda::math;

namespace pgeditor::gui
{
VerticalTileLayout::VerticalTileLayout(std::shared_ptr<Tile> parentTile) : TileLayout(parentTile) {}
VerticalTileLayout::~VerticalTileLayout() {}

void VerticalTileLayout::Layout(const std::vector<std::shared_ptr<Tile>>& tiles)
{
	auto parentPos = m_parentTile->GetPosition();
	auto parentSize = m_parentTile->GetSize();

	auto width = X(parentSize);
	auto height = static_cast<float>(Y(parentSize)) / tiles.size();
	auto currentPos = Y(parentPos);
	for (auto& t : tiles) {
		t->SetSize({static_cast<uint32_t>(width), static_cast<uint32_t>(height)});
		t->SetPosition({X(parentPos), currentPos});
		currentPos += height;
	}
}
}  // namespace pgeditor::gui
