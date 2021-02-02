#pragma once

#include <string>

namespace pgeditor::event
{
class Event
{
	public:
	virtual ~Event() {}

	virtual const std::string& GetName() const = 0;
};
}  // namespace pgeditor::event
