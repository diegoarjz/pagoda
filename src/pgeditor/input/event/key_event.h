#pragma once

#include <pgeditor/event/event.h>

#include <pgeditor/input/keys.h>

namespace pgeditor::input::event
{
class KeyEvent final : public pgeditor::event::Event
{
	public:
	enum class KeyEventType
	{
		Pressed,
		Released,
		Repeat
	};

	using Key = pgeditor::input::Key;

	KeyEvent(int key, int scancode, int mods, KeyEventType type);
	~KeyEvent() override;

	const std::string& GetName() const override;

	Key GetKey() const;
	int GetScanCode() const;
	int GetMods() const;
	KeyEventType GetType() const;

	private:
	Key m_key;
	int m_scancode;
	int m_mods;
	KeyEventType m_eventType;
};
}  // namespace pgeditor::input::event
