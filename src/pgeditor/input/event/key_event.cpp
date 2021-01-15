#include "key_event.h"

namespace pgeditor::input::event
{
KeyEvent::KeyEvent(int key, int scancode, int mods, KeyEventType type)
  : m_key{static_cast<Key>(key)}, m_scancode{scancode}, m_mods{mods}, m_eventType{type}
{
	//
}
KeyEvent::~KeyEvent()
{
	//
}

const std::string& KeyEvent::GetName() const
{
	static const std::string kName{"KeyEvent"};
	return kName;
}

Key KeyEvent::GetKey() const { return m_key; }
int KeyEvent::GetScanCode() const { return m_scancode; }
int KeyEvent::GetMods() const { return m_mods; }
KeyEvent::KeyEventType KeyEvent::GetType() const { return m_eventType; }
}  // namespace pgeditor::input::event
