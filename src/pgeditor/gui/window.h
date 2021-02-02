#pragma once

#include <pagoda/math/vec_base.h>

#include <memory>

namespace pgeditor::gui
{
class Tile;

class Window
{
	public:
	Window();
	Window(std::shared_ptr<Tile> parentTile);
	virtual ~Window();

	void SetParentTile(std::shared_ptr<Tile> tile);
	std::shared_ptr<Tile> GetTitle() const;

	const pagoda::math::Vec2U& GetSize() const;
	const pagoda::math::Vec2U& GetPosition() const;

	virtual void Render();

	private:
	std::string m_windowName;
	std::weak_ptr<Tile> m_parentTile;

	static std::size_t nextWindowId;
};
}  // namespace pgeditor::gui
