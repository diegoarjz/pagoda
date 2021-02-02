#pragma once

#include <pgeditor/event/event.h>

namespace pgeditor::window::event
{
class QuitEvent : public pgeditor::event::Event
{
	public:
	QuitEvent();
	~QuitEvent() override;

	const std::string& GetName() const override;
};
}  // namespace pgeditor::window::event
