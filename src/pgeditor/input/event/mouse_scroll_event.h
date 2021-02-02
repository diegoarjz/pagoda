#pragma once

#include <pgeditor/event/event.h>

#include <Magnum/Math/Vector2.h>

namespace pgeditor::input::event
{
class MouseScrollEvent final : public pgeditor::event::Event
{
	public:
	MouseScrollEvent(const Magnum::Math::Vector2<int>& offset, const Magnum::Math::Vector2<int> pos);
	~MouseScrollEvent() override;

	const std::string& GetName() const override;

	const Magnum::Math::Vector2<int>& offset() const { return m_offset; }
	const Magnum::Math::Vector2<int>& position() const { return m_mousePos; }

	private:
	Magnum::Math::Vector2<int> m_offset;
	Magnum::Math::Vector2<int> m_mousePos;
};
}  // namespace pgeditor::input::event
