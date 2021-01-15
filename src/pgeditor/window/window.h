#pragma once

#include <pgeditor/input/keys.h>
#include <pgeditor/input/mouse.h>

#include <functional>
#include <memory>

namespace pgeditor::event
{
class EventManager;
}

namespace pgeditor::window
{
class WindowCreationParams;

/**
 * The main \c Window for the editor.
 */
class Window
{
	public:
	Window();
	~Window();

	void SetEventManager(event::EventManager *eventManager);

	void Create(const WindowCreationParams &parameters);
	bool GetWindowShouldClose() const;
	void MakeContextCurrent();
	void Destroy();
	void SwapBuffers();

	void PollEvents();

	double GetEllapsedTime() const;

	void EnterCursorRawMode();
	void ExitCursorRawMode();

	struct Impl;

	private:
	std::unique_ptr<Impl> m_implementation;
};

using WindowPtr = std::shared_ptr<Window>;
}  // namespace pgeditor::window
