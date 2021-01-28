#include <pgeditor/gui/tile.h>

#include <pagoda/math/vec_base.h>

#include <gtest/gtest.h>

#include <iostream>

using namespace pagoda;
using namespace pagoda::math;

using namespace pgeditor;
using namespace pgeditor::gui;

class TileTest : public ::testing::Test
{
	protected:
	void SetUp() override { m_rootTile = std::make_shared<Tile>(Vec2U{0u, 0u}, Vec2U{100u, 200u}); }

	void TearDown() override
	{
		//
	}

	std::shared_ptr<Tile> m_rootTile;
};

TEST_F(TileTest, test_root_tile_construction)
{
	EXPECT_TRUE(m_rootTile->GetSize() == (Vec2U{100u, 200u}));
	EXPECT_EQ(m_rootTile->GetSplitDirection(), Tile::SplitDirection::None);
	EXPECT_EQ(m_rootTile->GetParent(), nullptr);
	EXPECT_NE(m_rootTile->GetAttachedWindow(), nullptr);
	EXPECT_NE(std::make_shared<Tile>()->GetAttachedWindow(), nullptr);
}

TEST_F(TileTest, test_horizontal_split)
{
	m_rootTile->Split(Tile::SplitDirection::Horizontal);
	EXPECT_EQ(m_rootTile->GetChildren().size(), 2u);
	for (auto c : m_rootTile->GetChildren()) {
		EXPECT_EQ(c->GetParent(), m_rootTile);
		EXPECT_TRUE(c->GetSize() == (Vec2U{50u, 200u}));
	}
	EXPECT_TRUE(m_rootTile->GetChildren()[0]->GetPosition() == (Vec2U{0u, 0u}));
	EXPECT_TRUE(m_rootTile->GetChildren()[1]->GetPosition() == (Vec2U{50u, 0u}));
}

TEST_F(TileTest, test_vertical_split)
{
	m_rootTile->Split(Tile::SplitDirection::Vertical);
	EXPECT_EQ(m_rootTile->GetChildren().size(), 2u);
	for (auto c : m_rootTile->GetChildren()) {
		EXPECT_EQ(c->GetParent(), m_rootTile);
		EXPECT_TRUE(c->GetSize() == (Vec2U{100u, 100u}));
	}
	EXPECT_TRUE(m_rootTile->GetChildren()[0]->GetPosition() == (Vec2U{0u, 0u}));
	EXPECT_TRUE(m_rootTile->GetChildren()[1]->GetPosition() == (Vec2U{0u, 100u}));
}

TEST_F(TileTest, test_resize_horizontal_split)
{
	m_rootTile->Split(Tile::SplitDirection::Horizontal);
	m_rootTile->SetSize(200u, 200u);

	auto children = m_rootTile->GetChildren();
	EXPECT_EQ(children[0]->GetWidth(), 100u);
	EXPECT_EQ(children[0]->GetHeight(), 200u);
	EXPECT_EQ(children[1]->GetWidth(), 100u);
	EXPECT_EQ(children[1]->GetHeight(), 200u);

	EXPECT_TRUE(m_rootTile->GetChildren()[0]->GetPosition() == (Vec2U{0u, 0u}));
	EXPECT_TRUE(m_rootTile->GetChildren()[1]->GetPosition() == (Vec2U{100u, 0u}));
}

TEST_F(TileTest, test_resize_vertical_split)
{
	m_rootTile->Split(Tile::SplitDirection::Vertical);
	m_rootTile->SetSize(100u, 100u);

	auto children = m_rootTile->GetChildren();
	EXPECT_EQ(children[0]->GetWidth(), 100u);
	EXPECT_EQ(children[0]->GetHeight(), 50u);
	EXPECT_EQ(children[1]->GetWidth(), 100u);
	EXPECT_EQ(children[1]->GetHeight(), 50u);

	EXPECT_TRUE(m_rootTile->GetChildren()[0]->GetPosition() == (Vec2U{0u, 0u}));
	EXPECT_TRUE(m_rootTile->GetChildren()[1]->GetPosition() == (Vec2U{0, 50u}));
}

TEST_F(TileTest, test_nested_resizes)
{
	m_rootTile->Split(Tile::SplitDirection::Horizontal);
	m_rootTile->GetChildren()[1]->Split(Tile::SplitDirection::Vertical);
	m_rootTile->SetSize(200u, 200u);

	auto left = m_rootTile->GetChildren()[0];
	auto right = m_rootTile->GetChildren()[1];
	auto topRight = right->GetChildren()[0];
	auto bottomRight = right->GetChildren()[1];

	EXPECT_EQ(topRight->GetWidth(), 100u);
	EXPECT_EQ(topRight->GetHeight(), 100u);
	EXPECT_TRUE(topRight->GetPosition() == (Vec2U{100u, 0u}));
	EXPECT_EQ(bottomRight->GetWidth(), 100u);
	EXPECT_EQ(bottomRight->GetHeight(), 100u);
	EXPECT_TRUE(bottomRight->GetPosition() == (Vec2U{100u, 100u}));
}
