#include "mouse_scroll_event.h"

namespace pgeditor::input::event
{
MouseScrollEvent::MouseScrollEvent(const Magnum::Math::Vector2<int>& offset, const Magnum::Math::Vector2<int> pos)
  : m_offset{offset}, m_mousePos{pos}
{
}
MouseScrollEvent::~MouseScrollEvent()
{
	//
}

const std::string& MouseScrollEvent::GetName() const
{
	static const std::string kName{"MouseScrollEvent"};
	return kName;
}
}  // namespace pgeditor::input::event
