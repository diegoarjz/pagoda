#include "quit_event.h"

namespace pgeditor::window::event
{
QuitEvent::QuitEvent() {}
QuitEvent::~QuitEvent() {}

const std::string& QuitEvent::GetName() const
{
	static const std::string kName{"QuitEvent"};
	return kName;
}
}  // namespace pgeditor::window::event
