#include "input_manager.h"

#include <pagoda/common/debug/logger.h>
#include <pagoda/common/instrument/profiler.h>

#include <boost/signals2.hpp>

#include <unordered_map>
#include <unordered_set>

namespace pgeditor::input
{
class InputManager::Impl
{
	public:
	using KeyMapContainer_t = std::unordered_map<std::size_t, boost::signals2::signal<void(void)>>;
	using MouseDragMapContainer_t = std::unordered_map<
	  std::size_t, boost::signals2::signal<void(const MouseButton &button, int xPos, int yPos, int xDelta, int yDelta)>>;
	~Impl() {}

	void AddKeyMapping(const KeyMapping &mapping)
	{
		auto k = makeKey(mapping.m_modifiers, mapping.m_key);
		switch (mapping.m_action) {
			case OnPress:
				m_onPressSignals[k].connect(mapping.m_handler);
				break;
			case OnRelease:
				m_onReleaseSignals[k].connect(mapping.m_handler);
				break;
			case OnRepeat:
				m_onRepeatSignals[k].connect(mapping.m_handler);
				break;
			case WhilePressed:
				m_whilePressedSignals[k].connect(mapping.m_handler);
				break;
		};
	}
	void AddMouseMovedHandler(const MouseMovedHandler_t &handler) { m_mouseMovedSignal.connect(handler); }
	void AddMouseDragHandler(const MouseDragMapping &mapping)
	{
		auto k = makeKey(mapping.m_modifiers, mapping.m_button);
		m_onMouseDragSignals[k].connect(mapping.m_handler);
	}

	void OnKeyPressed(const Key &k, int scanCode)
	{
		switch (k) {
			case Key::LeftShift:
			case Key::RightShift:
				m_modifiers |= (1 << static_cast<uint8_t>(Mods::Shift));
				break;
			case Key::LeftControl:
			case Key::RightControl:
				m_modifiers |= (1 << static_cast<uint8_t>(Mods::Control));
				break;
			case Key::LeftAlt:
			case Key::RightAlt:
				m_modifiers |= (1 << static_cast<uint8_t>(Mods::Alt));
				break;
			case Key::LeftSuper:
			case Key::RightSuper:
				m_modifiers |= (1 << static_cast<uint8_t>(Mods::Super));
				break;
			default:
				break;
		};

		m_pressedKeys.insert(k);
	}

	void OnKeyReleased(const Key &k, int scanCode)
	{
		switch (k) {
			case Key::LeftShift:
			case Key::RightShift:
				m_modifiers &= ~(1 << static_cast<uint8_t>(Mods::Shift));
				break;
			case Key::LeftControl:
			case Key::RightControl:
				m_modifiers &= ~(1 << static_cast<uint8_t>(Mods::Control));
				break;
			case Key::LeftAlt:
			case Key::RightAlt:
				m_modifiers &= ~(1 << static_cast<uint8_t>(Mods::Alt));
				break;
			case Key::LeftSuper:
			case Key::RightSuper:
				m_modifiers &= (1 << static_cast<uint8_t>(Mods::Super));
				break;
			default:
				break;
		};
		m_releasedKeys.insert(k);
	}

	void OnKeyRepeat(const Key &k, int scanCode) { m_repeatedKeys.insert(k); }

	void OnMouseMoved(int xPos, int yPos, int xDelta, int yDelta)
	{
		m_mouseMoved = true;
		m_mouseXPos = xPos;
		m_mouseYPos = yPos;
		m_mouseXDelta += xDelta;
		m_mouseYDelta += yDelta;
	}

	void OnMouseDrag(const MouseButton &button, int xPos, int yPos, int xDelta, int yDelta)
	{
		m_draggedMouseButtons.insert(button);
	}

	bool Init() { return true; }
	void Update(double dT)
	{
		START_PROFILE;
		for (const auto &k : m_pressedKeys) {
			// Fire pressed
			fireSignal(m_onPressSignals, k);
		}
		for (const auto &k : m_releasedKeys) {
			// Fire released
			fireSignal(m_onReleaseSignals, k);
			m_whilePressedKeys.erase(k);
			m_pressedKeys.erase(k);
		}
		for (const auto &k : m_repeatedKeys) {
			// Fire repeated
			fireSignal(m_onRepeatSignals, k);
		}
		for (const auto &k : m_whilePressedKeys) {
			// Fire while pressed
			fireSignal(m_whilePressedSignals, k);
		}
		for (const auto &k : m_pressedKeys) {
			m_whilePressedKeys.insert(k);
		}
		m_pressedKeys.clear();
		m_releasedKeys.clear();
		m_repeatedKeys.clear();

		if (m_mouseMoved) {
			m_mouseMovedSignal(m_mouseXPos, m_mouseYPos, m_mouseXDelta, m_mouseYDelta);
		}

		for (const auto &b : m_draggedMouseButtons) {
			fireSignal(m_onMouseDragSignals, b, b, m_mouseXPos, m_mouseYPos, m_mouseXDelta, m_mouseYDelta);
		}
		m_draggedMouseButtons.clear();

		m_mouseXPos = 0;
		m_mouseYPos = 0;
		m_mouseXDelta = 0;
		m_mouseYDelta = 0;
		m_mouseMoved = false;
	}
	void Destroy() {}

	private:
	template<typename T>
	std::size_t makeKey(uint8_t mods, T k)
	{
		return (mods << (sizeof(std::size_t) / 8)) | static_cast<std::size_t>(k);
	}

	template<typename T, typename K, class... Args>
	void fireSignal(T &container, const K &k, Args... args)
	{
		auto mapKey = makeKey(m_modifiers, k);
		auto iter = container.find(mapKey);
		if (iter != container.end()) {
			iter->second(args...);
		}
	}

	bool m_mouseMoved;
	int m_mouseXPos;
	int m_mouseYPos;
	int m_mouseXDelta;
	int m_mouseYDelta;

	uint8_t m_modifiers;
	std::unordered_set<Key> m_pressedKeys;
	std::unordered_set<Key> m_releasedKeys;
	std::unordered_set<Key> m_repeatedKeys;
	std::unordered_set<Key> m_whilePressedKeys;
	std::unordered_set<MouseButton> m_draggedMouseButtons;

	boost::signals2::signal<void(int, int, int, int)> m_mouseMovedSignal;

	KeyMapContainer_t m_onPressSignals;
	KeyMapContainer_t m_onReleaseSignals;
	KeyMapContainer_t m_onRepeatSignals;
	KeyMapContainer_t m_whilePressedSignals;
	MouseDragMapContainer_t m_onMouseDragSignals;
};

InputManager::InputManager() : m_implementation(std::make_unique<Impl>()) {}
InputManager::~InputManager() {}

void InputManager::AddKeyMapping(const KeyMapping &mapping) { m_implementation->AddKeyMapping(mapping); }
void InputManager::AddMouseMovedHandler(const MouseMovedHandler_t &handler)
{
	m_implementation->AddMouseMovedHandler(handler);
}

void InputManager::OnKeyPressed(const Key &k, int scanCode) { m_implementation->OnKeyPressed(k, scanCode); }
void InputManager::OnKeyReleased(const Key &k, int scanCode) { m_implementation->OnKeyReleased(k, scanCode); }
void InputManager::OnKeyRepeat(const Key &k, int scanCode) { m_implementation->OnKeyRepeat(k, scanCode); }

void InputManager::OnMouseMoved(int xPos, int yPos, int xDelta, int yDelta)
{
	m_implementation->OnMouseMoved(xPos, yPos, xDelta, yDelta);
}

void InputManager::OnMouseDrag(const MouseButton &button, int xPos, int yPos, int xDelta, int yDelta)
{
	m_implementation->OnMouseDrag(button, xPos, yPos, xDelta, yDelta);
}

void InputManager::AddMouseDragHandler(const MouseDragMapping &handler)
{
	m_implementation->AddMouseDragHandler(handler);
}

bool InputManager::Init() { return m_implementation->Init(); }
void InputManager::Update(double dT) { m_implementation->Update(dT); }
void InputManager::Destroy() { m_implementation->Destroy(); }

}  // namespace pgeditor::input
