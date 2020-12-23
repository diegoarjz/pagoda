#include "window.h"

#include <pagoda/common/debug/logger.h>

#include "window_creation_params.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <boost/signals2.hpp>

#include <iostream>
#include <unordered_map>

using namespace pgeditor::input;
using namespace boost;

namespace pgeditor::window
{
Key convertKeyFromGLFW(int k) { return static_cast<Key>(k); }
MouseButton convertMouseFromGLFW(int m) { return static_cast<MouseButton>(m); }

static void errorCallback(int errorCode, const char* errorDescription);
static void windowSizeCallback(GLFWwindow* w, int width, int height);
static void cursorPosCallBack(GLFWwindow* window, double xPos, double yPos);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

struct Window::Impl
{
	Impl() : m_window(nullptr) {}
	~Impl() { Destroy(); }

	bool GetSize(uint32_t* outWidth, uint32_t* outHeight)
	{
		if (!m_window)
		{
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
		if (!m_window)
		{
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
		if (m_window)
		{
			glfwSetWindowTitle(m_window, title.c_str());
		}
	}

	void Create(const WindowCreationParams& params)
	{
		auto pixelFormat = params.GetPixelFormat();
		auto contextVersion = params.GetContextVersion();

		if (m_window)
		{
			LOG_FATAL("Window creation requested but window already created.");
		}

		glfwSetErrorCallback(errorCallback);

		if (!glfwInit())
		{
			LOG_FATAL("Failed to initialize GLFW");
		}

		for (uint32_t i = 0; i < 4; ++i)
		{
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

		if (!m_window)
		{
			LOG_FATAL("Failed to create window");
			glfwTerminate();
		}

		glfwSetCursorPosCallback(m_window, cursorPosCallBack);
		glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
		glfwSetScrollCallback(m_window, scrollCallback);
		glfwSetKeyCallback(m_window, keyCallback);
		glfwSetWindowSizeCallback(m_window, windowSizeCallback);

		s_glfwToWindowMap[m_window] = this;

		MakeContextCurrent();

		glewExperimental = true;
		if (glewInit() != GLEW_OK)
		{
			LOG_FATAL("Failed to initialize GLEW");
			glfwTerminate();
		}

		glfwSetWindowPos(m_window, params.GetXPos(), params.GetYPos());
	}

	void MakeContextCurrent()
	{
		if (!m_window)
		{
			return;
		}
		glfwMakeContextCurrent(m_window);
	}

	bool GetWindowShouldClose() const { return m_window && glfwWindowShouldClose(m_window); }

	void Destroy()
	{
		if (m_window)
		{
			glfwDestroyWindow(m_window);
			m_window = nullptr;
		}
		glfwTerminate();
	}

	void SwapBuffers()
	{
		if (m_window)
		{
			glfwSwapBuffers(m_window);
		}
	}

	void PollEvents() { glfwPollEvents(); }

	double GetEllapsedTime() const { return glfwGetTime(); }

	void CursorPositionChanged(int xPos, int yPos)
	{
		auto xDelta = xPos - m_mouseXPos;
		auto yDelta = yPos - m_mouseYPos;
		m_mouseMoved(xPos, yPos, xDelta, yDelta);

		for (auto i = 0u; i < GLFW_MOUSE_BUTTON_LAST; ++i)
		{
			if (glfwGetMouseButton(m_window, i) == GLFW_PRESS)
			{
				m_drag(convertMouseFromGLFW(i), xPos, yPos, xDelta, yDelta);
			}
		}

		m_mouseXPos = xPos;
		m_mouseYPos = yPos;
	}

	void WindowResize(int width, int height) { m_windowResize(width, height); }

	void KeyPressed(int key, int scancode, int mods) { m_keyPressed(convertKeyFromGLFW(key), scancode); }

	void KeyReleased(int key, int scancode, int mods) { m_keyReleased(convertKeyFromGLFW(key), scancode); }

	void KeyRepeat(int key, int scancode, int mods) { m_keyRepeat(convertKeyFromGLFW(key), scancode); }

	void MouseButtonPressed(int button, int mods) { m_mouseButtonPressed(convertMouseFromGLFW(button)); }

	void MouseButtonReleased(int button, int mods) { m_mouseButtonReleased(convertMouseFromGLFW(button)); }

	void Scroll(double xOffset, double yOffset) { m_scrolled(xOffset, yOffset); }

	void RegisterOnWindowResize(const std::function<void(int, int)>& handler) { m_windowResize.connect(handler); }

	void RegisterOnDrag(const std::function<void(MouseButton, int, int, int, int)>& handler)
	{
		m_drag.connect(handler);
	}
	void RegisterOnKeyPressed(const std::function<void(Key, int)>& handler) { m_keyPressed.connect(handler); }
	void RegisterOnKeyReleased(const std::function<void(Key, int)>& handler) { m_keyReleased.connect(handler); }
	void RegisterOnKeyRepeat(const std::function<void(Key, int)>& handler) { m_keyRepeat.connect(handler); }
	void RegisterOnMouseMoved(const std::function<void(int, int, int, int)>& handler) { m_mouseMoved.connect(handler); }
	void RegisterOnMouseButtonPressed(const std::function<void(MouseButton)>& handler)
	{
		m_mouseButtonPressed.connect(handler);
	}
	void RegisterOnMouseButtonReleased(const std::function<void(MouseButton)>& handler)
	{
		m_mouseButtonReleased.connect(handler);
	}
	void RegisterOnScroll(const std::function<void(double, double)>& handler) { m_scrolled.connect(handler); }

	void EnterCursorRawMode() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
	void ExitCursorRawMode() { glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

	static Impl* GetMappedWindow(GLFWwindow* w) { return s_glfwToWindowMap[w]; }

private:
	GLFWwindow* m_window;

	int m_mouseXPos;
	int m_mouseYPos;

	signals2::signal<void(int, int, int, int)> m_mouseMoved;
	signals2::signal<void(MouseButton)> m_mouseButtonPressed;
	signals2::signal<void(MouseButton)> m_mouseButtonReleased;
	signals2::signal<void(double, double)> m_scrolled;
	signals2::signal<void(Key, int)> m_keyPressed;
	signals2::signal<void(Key, int)> m_keyReleased;
	signals2::signal<void(Key, int)> m_keyRepeat;
	signals2::signal<void(MouseButton, int, int, int, int)> m_drag;
	signals2::signal<void(int, int)> m_windowResize;

	static std::unordered_map<GLFWwindow*, Impl*> s_glfwToWindowMap;
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
	switch (action)
	{
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

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
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

void Window::RegisterOnWindowResize(const std::function<void(int, int)>& handler)
{
	m_implementation->RegisterOnWindowResize(handler);
}

void Window::RegisterOnKeyPressed(const std::function<void(Key, int)>& handler)
{
	m_implementation->RegisterOnKeyPressed(handler);
}
void Window::RegisterOnKeyReleased(const std::function<void(Key, int)>& handler)
{
	m_implementation->RegisterOnKeyReleased(handler);
}
void Window::RegisterOnKeyRepeat(const std::function<void(Key, int)>& handler)
{
	m_implementation->RegisterOnKeyRepeat(handler);
}
void Window::RegisterOnMouseMoved(const std::function<void(int, int, int, int)>& handler)
{
	m_implementation->RegisterOnMouseMoved(handler);
}
void Window::RegisterOnMouseButtonPressed(const std::function<void(MouseButton)>& handler)
{
	m_implementation->RegisterOnMouseButtonPressed(handler);
}
void Window::RegisterOnMouseButtonReleased(const std::function<void(MouseButton)>& handler)
{
	m_implementation->RegisterOnMouseButtonReleased(handler);
}
void Window::RegisterOnScroll(const std::function<void(double, double)>& handler)
{
	m_implementation->RegisterOnScroll(handler);
}

void Window::RegisterOnDrag(const std::function<void(MouseButton, int, int, int, int)>& handler)
{
	m_implementation->RegisterOnDrag(handler);
}

void Window::EnterCursorRawMode() { m_implementation->EnterCursorRawMode(); }
void Window::ExitCursorRawMode() { m_implementation->ExitCursorRawMode(); }
}  // namespace pgeditor::window
