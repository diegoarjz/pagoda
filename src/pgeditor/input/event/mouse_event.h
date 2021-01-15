#pragma once

#include <pgeditor/event/event.h>

#include <Magnum/Math/Vector2.h>

namespace pgeditor::input::event
{
class MouseEvent final : public pgeditor::event::Event
{
	public:
	enum class Button
	{
		Left,
		Middle,
		Right
	};

	MouseEvent(const Magnum::Math::Vector2<int>& pos, Button button, bool pressed);
	~MouseEvent() override;

	const std::string& GetName() const override;

	Button button() const { return m_button; }
	const Magnum::Math::Vector2<int>& position() const { return m_mousePos; }
	bool pressed() const { return m_pressed; }

	private:
	Magnum::Math::Vector2<int> m_mousePos;
	Button m_button;
	bool m_pressed;
};
}  // namespace pgeditor::input::event
