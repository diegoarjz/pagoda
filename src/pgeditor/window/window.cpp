#include "window.h"

#include "window_creation_params.h"

#include <pgeditor/event/event_manager.h>
#include <pgeditor/input/event/character_event.h>
#include <pgeditor/input/event/key_event.h>
#include <pgeditor/input/event/mouse_event.h>
#include <pgeditor/input/event/mouse_move_event.h>
#include <pgeditor/input/event/mouse_scroll_event.h>

#include <pagoda/common/debug/logger.h>

#include <Magnum/Platform/GLContext.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <boost/signals2.hpp>

#include <iostream>
#include <unordered_map>

using namespace pgeditor::input;
using namespace pgeditor::input::event;
using namespace pgeditor::event;

using namespace boost;
using namespace Magnum;

namespace pgeditor::window
{
Key convertKeyFromGLFW(int k) { return static_cast<Key>(k); }
MouseButton convertMouseFromGLFW(int m) { return static_cast<MouseButton>(m); }

static void errorCallback(int errorCode, const char* errorDescription);
static void windowSizeCallback(GLFWwindow* w, int width, int height);
static void cursorPosCallBack(GLFWwindow* window, double xPos, double yPos);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void characterCallBack(GLFWwindow* window, unsigned int keyPoint);
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

struct Window::Impl
{
	Impl() : m_window(nullptr) {}
	~Impl() { Destroy(); }

	void SetEventManager(EventManager* eventManager) { m_eventManager = eventManager; }

	bool GetSize(uint32_t* outWidth, uint32_t* outHeight)
	{
		if (!m_window) {
			return false;
		}

		int32_t width;
		int32_t height;

		glfwGetWindowSize(m_window, &width, &height);

		*outWidth = width;
		*outHeight = height;

		return true;
	}

	bool GetFrameBufferSize(uint32_t* outWidth, uint32_t* outHeight)
	{
		if (!m_window) {
			return false;
		}

		int32_t width;
		int32_t height;

		glfwGetFramebufferSize(m_window, &width, &height);

		*outWidth = width;
		*outHeight = height;

		return true;
	}

	void SetTitle(const std::string& title)
	{
		if (m_window) {
			glfwSetWindowTitle(m_window, title.c_str());
		}
	}

	void Create(const WindowCreationParams& params)
	{
		auto pixelFormat = params.GetPixelFormat();
		auto contextVersion = params.GetContextVersion();

		if (m_window) {
			LOG_FATAL("Window creation requested but window already created.");
		}

		glfwSetErrorCallback(errorCallback);

		if (!glfwInit()) {
			LOG_FATAL("Failed to initialize GLFW");
		}

		for (uint32_t i = 0; i < 4; ++i) {
			glfwWindowHint(GLFW_RED_BITS + i, pixelFormat[i]);
		}
		glfwWindowHint(GLFW_DEPTH_BITS, params.GetDepthBits());
		glfwWindowHint(GLFW_STENCIL_BITS, params.GetStencilBits());
		glfwWindowHint(GLFW_SAMPLES, params.GetAntialiasSamples());
		glfwWindowHint(GLFW_DOUBLEBUFFER, params.GetDoubleBuffer());
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, contextVersion[0]);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, contextVersion[1]);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, params.GetDebugContext());
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		m_window = glfwCreateWindow(params.GetWidth(), params.GetHeight(), params.GetTitle().c_str(), NULL, NULL);

		if (!m_window) {
			LOG_FATAL("Failed to create window.");
			glfwTerminate();
			return;
		}

		glfwSetCursorPosCallback(m_window, cursorPosCallBack);
		glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
		glfwSetScrollCallback(m_window, scrollCallback);
		glfwSetKeyCallback(m_window, keyCallback);
		glfwSetCharCallback(m_window, characterCallBack);
		glfwSetWindowSizeCallback(m_window, windowSizeCallback);

		s_glfwToWindowMap[m_window] = this;

		MakeContextCurrent();

		if (!m_context.tryCreate()) {
			LOG_FATAL("Failed to create Magnum context.");
			glfwTerminate();
			return;
		}

		glfwSetWindowPos(m_window, params.GetXPos(), params.GetYPos());
	}

	void MakeContextCurrent()
	{
		if (!m_window) {
			return;
		}
		glfwMakeContextCurrent(m_window);
	}

	bool GetWindowShouldClose() const { return m_window && glfwWindowShouldClose(m_window); }

	void Destroy()
	{
		if (m_window) {
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
		glfwTerminate();
	}

	void SwapBuffers()
	{
		if (m_window) {
			glfwSwapBuffers(m_window);
		}
	}

	void PollEvents() { glfwPollEvents(); }

	double GetEllapsedTime() const { return glfwGetTime(); }

	void CursorPositionChanged(int xPos, int yPos)
	{
		m_mouseXPos = xPos;
		m_mouseYPos = yPos;

		m_eventManager->PushEvent<MouseMoveEvent>(Magnum::Math::Vector2<int>{m_mouseXPos, m_mouseYPos});
	}

	void WindowResize(int width, int height) { /*m_windowResize(width, height);*/ }

	void KeyPressed(int key, int scancode, int mods)
	{
		m_eventManager->PushEvent<KeyEvent>(key, scancode, mods, input::event::KeyEvent::KeyEventType::Pressed);
	}

	void CharacterPressed(unsigned int keyPoint) { m_eventManager->PushEvent<CharacterEvent>(keyPoint); }

	void KeyReleased(int key, int scancode, int mods)
	{
		m_eventManager->PushEvent<KeyEvent>(key, scancode, mods, input::event::KeyEvent::KeyEventType::Released);
	}

	void KeyRepeat(int key, int scancode, int mods)
	{
		m_eventManager->PushEvent<KeyEvent>(key, scancode, mods, input::event::KeyEvent::KeyEventType::Repeat);
	}

	void MouseButtonPressed(int button, int mods)
	{
		m_eventManager->PushEvent<MouseEvent>(Magnum::Math::Vector2<int>{m_mouseXPos, m_mouseYPos},
		                                      static_cast<input::event::MouseEvent::Button>(button), true);
	}

	void MouseButtonReleased(int button, int mods)
	{
		m_eventManager->PushEvent<MouseEvent>(Magnum::Math::Vector2<int>{m_mouseXPos, m_mouseYPos},
		                                      static_cast<input::event::MouseEvent::Button>(button), false);
	}

	void Scroll(double xOffset, double yOffset)
	{
		m_eventManager->PushEvent<MouseScrollEvent>(
		  Magnum::Math::Vector2<int>{static_cast<int>(xOffset), static_cast<int>(yOffset)},
		  Magnum::Math::Vector2<int>{m_mouseXPos, m_mouseYPos});
	}

	void EnterCursorRawMode() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
	void ExitCursorRawMode() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	static Impl* GetMappedWindow(GLFWwindow* w) { return s_glfwToWindowMap[w]; }

	private:
	GLFWwindow* m_window;

	Platform::GLContext m_context{NoCreate};

	int m_mouseXPos;
	int m_mouseYPos;

	static std::unordered_map<GLFWwindow*, Impl*> s_glfwToWindowMap;

	EventManager* m_eventManager;
};  // struct Window::Impl

std::unordered_map<GLFWwindow*, Window::Impl*> Window::Impl::s_glfwToWindowMap;

static void errorCallback(int errorCode, const char* errorDescription)
{
	LOG_FATAL("GLFW raised an error");
	LOG_FATAL("\tError Code: 0x" << errorCode);
	LOG_FATAL("\tError Description: " << errorDescription);
}

static void windowSizeCallback(GLFWwindow* w, int width, int height)
{
	Window::Impl::GetMappedWindow(w)->WindowResize(width, height);
}

static void cursorPosCallBack(GLFWwindow* window, double xPos, double yPos)
{
	Window::Impl::GetMappedWindow(window)->CursorPositionChanged(static_cast<int>(xPos), static_cast<int>(yPos));
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action) {
		case GLFW_PRESS:
			Window::Impl::GetMappedWindow(window)->KeyPressed(key, scancode, mods);
			break;
		case GLFW_RELEASE:
			Window::Impl::GetMappedWindow(window)->KeyReleased(key, scancode, mods);
			break;
		case GLFW_REPEAT:
			Window::Impl::GetMappedWindow(window)->KeyRepeat(key, scancode, mods);
			break;
	};
}

static void characterCallBack(GLFWwindow* window, unsigned int keyPoint)
{
	Window::Impl::GetMappedWindow(window)->CharacterPressed(keyPoint);
}

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action) {
		case GLFW_PRESS:
			Window::Impl::GetMappedWindow(window)->MouseButtonPressed(button, mods);
			break;
		case GLFW_RELEASE:
			Window::Impl::GetMappedWindow(window)->MouseButtonReleased(button, mods);
			break;
	}
}

static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Window::Impl::GetMappedWindow(window)->Scroll(xOffset, yOffset);
}

Window::Window() : m_implementation(std::make_unique<Window::Impl>()) {}

Window::~Window() {}

void Window::Create(const WindowCreationParams& params) { m_implementation->Create(params); }

bool Window::GetWindowShouldClose() const { return m_implementation->GetWindowShouldClose(); }
void Window::MakeContextCurrent() { m_implementation->MakeContextCurrent(); }
void Window::Destroy() { m_implementation->Destroy(); }
void Window::SwapBuffers() { m_implementation->SwapBuffers(); }
void Window::PollEvents() { m_implementation->PollEvents(); }
double Window::GetEllapsedTime() const { return m_implementation->GetEllapsedTime(); }

void Window::EnterCursorRawMode() { m_implementation->EnterCursorRawMode(); }
void Window::ExitCursorRawMode() { m_implementation->ExitCursorRawMode(); }
void Window::SetEventManager(EventManager* eventManager) { m_implementation->SetEventManager(eventManager); }

}  // namespace pgeditor::window
