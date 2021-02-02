#include "tile_layout.h"

namespace pgeditor::gui
{
TileLayout::TileLayout(std::shared_ptr<Tile> parentTile) : m_parentTile(parentTile) {}
TileLayout::~TileLayout() {}
}  // namespace pgeditor::gui
