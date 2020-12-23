#pragma once

#include "keys.h"
#include "mouse.h"

#include <cstdint>
#include <functional>
#include <memory>

namespace pgeditor::input
{
class InputManager final
{
public:
	enum Mods
	{
		Shift,
		Control,
		Alt,
		Super
	};

	enum Action
	{
		OnPress,
		OnRelease,
		OnRepeat,
		WhilePressed
	};

	using KeyMappingHandler_t = std::function<void(void)>;
	using MouseMovedHandler_t = std::function<void(int, int, int, int)>;
	using MouseDragHandler_t = std::function<void(MouseButton, int, int, int, int)>;

	struct KeyMapping
	{
		KeyMapping(uint8_t modifiers, Key k, Action action, const KeyMappingHandler_t &handler)
		    : m_modifiers(modifiers), m_key(k), m_action(action), m_handler(handler)
		{
		}

		uint8_t m_modifiers;
		Key m_key;
		Action m_action;

		KeyMappingHandler_t m_handler;
	};

	struct MouseDragMapping
	{
		MouseDragMapping(uint8_t modifiers, MouseButton b,
		                 const std::function<void(const MouseButton, int, int, int, int)> &handler)
		    : m_modifiers(modifiers), m_button(b), m_handler(handler)
		{
		}

		uint8_t m_modifiers;
		MouseButton m_button;

		MouseDragHandler_t m_handler;
	};

	InputManager();
	~InputManager();

	void AddKeyMapping(const KeyMapping &mapping);
	void AddMouseMovedHandler(const MouseMovedHandler_t &handler);
	void AddMouseDragHandler(const MouseDragMapping &handler);

	void OnKeyPressed(const Key &k, int scanCode);
	void OnKeyReleased(const Key &k, int scanCode);
	void OnKeyRepeat(const Key &k, int scanCode);

	void OnMouseMoved(int xPos, int yPos, int xDelta, int yDelta);
	void OnMouseButtonPressed(const MouseButton &button);
	void OnMouseButtonReleased(const MouseButton &button);
	void OnMouseDrag(const MouseButton &button, int xPos, int yPos, int xDelta, int yDelta);

	void Update(double dT);

private:
	class Impl;
	std::unique_ptr<Impl> m_implementation;
};
}  // namespace pgeditor::input
