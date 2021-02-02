#include "window_resize_event.h"

namespace pgeditor::window::event
{
WindowResizeEvent::WindowResizeEvent(uint32_t width, uint32_t height) : m_width{width}, m_height{height} {}

WindowResizeEvent::~WindowResizeEvent() {}

uint32_t WindowResizeEvent::GetWidth() const { return m_width; }
uint32_t WindowResizeEvent::GetHeight() const { return m_height; }

const std::string& WindowResizeEvent::GetName() const
{
	static const std::string kName{"WindowResizeEvent"};
	return kName;
}
}  // namespace pgeditor::window::event
