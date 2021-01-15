#include "character_event.h"

namespace pgeditor::input::event
{
CharacterEvent::CharacterEvent(unsigned int key) : m_character(key)
{
	//
}
CharacterEvent::~CharacterEvent()
{
	//
}

const std::string& CharacterEvent::GetName() const
{
	static const std::string kName{"CharacterEvent"};
	return kName;
}

unsigned int CharacterEvent::GetCharacter() const { return m_character; }
}  // namespace pgeditor::input::event

