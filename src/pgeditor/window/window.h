#pragma once

#include <pgeditor/input/keys.h>
#include <pgeditor/input/mouse.h>

#include <functional>
#include <memory>

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

	void Create(const WindowCreationParams &parameters);
	bool GetWindowShouldClose() const;
	void MakeContextCurrent();
	void Destroy();
	void SwapBuffers();

	void PollEvents();

	double GetEllapsedTime() const;

	void RegisterOnWindowResize(const std::function<void(int, int)> &handler);
	void RegisterOnKeyPressed(const std::function<void(input::Key, int)> &handler);
	void RegisterOnKeyReleased(const std::function<void(input::Key, int)> &handler);
	void RegisterOnKeyRepeat(const std::function<void(input::Key, int)> &handler);
	void RegisterOnMouseMoved(const std::function<void(int, int, int, int)> &handler);
	void RegisterOnMouseButtonPressed(const std::function<void(input::MouseButton)> &handler);
	void RegisterOnMouseButtonReleased(const std::function<void(input::MouseButton)> &handler);
	void RegisterOnScroll(const std::function<void(double, double)> &handler);
	void RegisterOnDrag(const std::function<void(input::MouseButton, int, int, int, int)> &handler);

	void EnterCursorRawMode();
	void ExitCursorRawMode();

	struct Impl;

private:
	std::unique_ptr<Impl> m_implementation;
};

using WindowPtr = std::shared_ptr<Window>;
}  // namespace pgeditor::window
