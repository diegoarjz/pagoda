#pragma once

#include <pgeditor/event/event.h>

namespace pgeditor::window::event
{
class WindowResizeEvent : public pgeditor::event::Event
{
	public:
	WindowResizeEvent(uint32_t width, uint32_t height);
	~WindowResizeEvent() override;

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;

	const std::string& GetName() const override;

	private:
	uint32_t m_width;
	uint32_t m_height;
};
}  // namespace pgeditor::window::event
