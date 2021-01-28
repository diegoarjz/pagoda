#include <pgeditor/gui/tile.h>
#include <pgeditor/gui/vertical_tile_layout.h>

#include <pagoda/math/vec_base.h>

#include <gtest/gtest.h>

#include <boost/qvm/vec_operations.hpp>

#include <iostream>

using namespace pagoda;
using namespace pagoda::math;

using namespace pgeditor;
using namespace pgeditor::gui;

class VerticalTileLayoutTest : public ::testing::Test
{
	protected:
	void SetUp() override
	{
		m_tile = std::make_shared<Tile>(Vec2U{100, 200}, Vec2U{200, 100});
		m_layout = std::make_shared<VerticalTileLayout>(m_tile);
	}

	void TearDown() override
	{
		//
	}

	std::shared_ptr<Tile> m_tile;
	std::shared_ptr<VerticalTileLayout> m_layout;
};

TEST_F(VerticalTileLayoutTest, test_single_tile_layout)
{
	auto tile = std::make_shared<Tile>();
	std::vector<std::shared_ptr<Tile>> tiles{tile};
	m_layout->Layout(tiles);

	EXPECT_TRUE(tile->GetPosition() == (Vec2U{100, 200}));
	EXPECT_TRUE(tile->GetSize() == (Vec2U{200, 100}));
}

TEST_F(VerticalTileLayoutTest, test_multiple_tile_layout)
{
	auto tile = std::make_shared<Tile>();
	auto tile2 = std::make_shared<Tile>();
	std::vector<std::shared_ptr<Tile>> tiles{tile, tile2};
	m_layout->Layout(tiles);

	EXPECT_TRUE(tile->GetPosition() == (Vec2U{100, 200}));
	EXPECT_TRUE(tile->GetSize() == (Vec2U{200, 50}));
	EXPECT_TRUE(tile2->GetPosition() == (Vec2U{100, 250}));
	EXPECT_TRUE(tile2->GetSize() == (Vec2U{200, 50}));
}
