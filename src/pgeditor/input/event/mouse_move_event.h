#pragma once

#include <pgeditor/event/event.h>

#include <Magnum/Math/Vector2.h>

namespace pgeditor::input::event
{
class MouseMoveEvent final : public pgeditor::event::Event
{
	public:
	MouseMoveEvent(const Magnum::Math::Vector2<int>& pos);
	~MouseMoveEvent() override;

	const std::string& GetName() const override;

	const Magnum::Math::Vector2<int>& position() const { return m_mousePos; }

	private:
	Magnum::Math::Vector2<int> m_mousePos;
};
}  // namespace pgeditor::input::event
