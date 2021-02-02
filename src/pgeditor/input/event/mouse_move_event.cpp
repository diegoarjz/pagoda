#include "mouse_move_event.h"

namespace pgeditor::input::event
{
MouseMoveEvent::MouseMoveEvent(const Magnum::Math::Vector2<int>& pos) : m_mousePos{pos} {}
MouseMoveEvent::~MouseMoveEvent() {}

const std::string& MouseMoveEvent::GetName() const
{
	static const std::string& kName{"MouseMoveEvent"};
	return kName;
}
}  // namespace pgeditor::input::event
