#include "mouse_event.h"

namespace pgeditor::input::event
{
MouseEvent::MouseEvent(const Magnum::Math::Vector2<int>& pos, Button button, bool pressed)
  : m_mousePos(pos), m_button{button}, m_pressed{pressed}
{
}

MouseEvent::~MouseEvent() {}

const std::string& MouseEvent::GetName() const
{
	static const std::string kName{"MouseEvent"};
	return kName;
}
}  // namespace pgeditor::input::event
