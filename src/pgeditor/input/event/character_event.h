#pragma once

#include <pgeditor/event/event.h>

namespace pgeditor::input::event
{
class CharacterEvent final : public pgeditor::event::Event
{
	public:
	CharacterEvent(unsigned int key);
	~CharacterEvent() override;

	const std::string& GetName() const override;

	unsigned int GetCharacter() const;

	private:
	unsigned int m_character;
};
}  // namespace pgeditor::input::event

