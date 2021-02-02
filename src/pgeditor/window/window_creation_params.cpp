#include "window_creation_params.h"

namespace pgeditor::window
{
struct WindowCreationParams::Impl
{
public:
	Impl()
	    : m_width(800),
	      m_height(600),
	      m_xPos(0),
	      m_yPos(0),
	      m_fullScreen(false),
	      m_pixelFormat{8, 8, 8, 8},
	      m_depthBits(24),
	      m_stencilBits(8),
	      m_antialiasingSamples(4),
	      m_doubleBuffer(true),
	      m_contextVersion{4, 1},
	      m_debugContext(false),
	      m_title("Window")
	{
	}

	void Size(uint32_t width, uint32_t height)
	{
		m_width = width;
		m_height = height;
	}

	void Position(uint32_t xPos, uint32_t yPos)
	{
		m_xPos = xPos;
		m_yPos = yPos;
	}

	void FullScreen(bool fullScreen) { m_fullScreen = fullScreen; }

	void PixelFormat(uint32_t redBits, uint32_t greenBits, uint32_t blueBits, uint32_t alphaBits)
	{
		m_pixelFormat[0] = redBits;
		m_pixelFormat[1] = greenBits;
		m_pixelFormat[2] = blueBits;
		m_pixelFormat[3] = alphaBits;
	}

	void DepthBits(uint32_t bits) { m_depthBits = bits; }

	void StencilBits(uint32_t bits) { m_stencilBits = bits; }

	void AntialiasSamples(uint32_t samples) { m_antialiasingSamples = samples; }

	void DoubleBuffer(bool doubleBuffer) { m_doubleBuffer = doubleBuffer; }

	bool GetDoubleBuffer() const { return m_doubleBuffer; }

	void ContextVersion(uint32_t major, uint32_t minor)
	{
		m_contextVersion[0] = major;
		m_contextVersion[1] = minor;
	}

	void DebugContext(bool debug) { m_debugContext = debug; }

	void Title(const std::string& title) { m_title = title; }

	uint32_t GetWidth() const { return m_width; }

	uint32_t GetHeight() const { return m_height; }

	uint32_t GetXPos() const { return m_xPos; }

	uint32_t GetYPos() const { return m_yPos; }

	bool GetFullScreen() const { return m_fullScreen; }

	const uint32_t* const GetPixelFormat() const { return m_pixelFormat; }

	uint32_t GetDepthBits() const { return m_depthBits; }

	uint32_t GetStencilBits() const { return m_stencilBits; }

	uint32_t GetAntialiasSamples() const { return m_antialiasingSamples; }

	const uint32_t* const GetContextVersion() const { return m_contextVersion; }

	bool GetDebugContext() const { return m_debugContext; }

	const std::string& GetTitle() const { return m_title; }

private:
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_xPos;
	uint32_t m_yPos;
	bool m_fullScreen;
	uint32_t m_pixelFormat[4];
	uint32_t m_depthBits;
	uint32_t m_stencilBits;
	uint32_t m_antialiasingSamples;
	bool m_doubleBuffer;
	uint32_t m_contextVersion[2];
	bool m_debugContext;
	std::string m_title;
};  // struct WindowCreationParams::Impl

WindowCreationParams::WindowCreationParams() : m_implementation(std::make_unique<WindowCreationParams::Impl>()) {}
WindowCreationParams::~WindowCreationParams() {}

WindowCreationParams& WindowCreationParams::Size(uint32_t width, uint32_t height)
{
	m_implementation->Size(width, height);
	return *this;
}

WindowCreationParams& WindowCreationParams::Position(uint32_t xPos, uint32_t yPos)
{
	m_implementation->Position(xPos, yPos);
	return *this;
}

WindowCreationParams& WindowCreationParams::FullScreen(bool fullScreen)
{
	m_implementation->FullScreen(fullScreen);
	return *this;
}

WindowCreationParams& WindowCreationParams::PixelFormat(uint32_t redBits, uint32_t greenBits, uint32_t blueBits,
                                                        uint32_t alphaBits)
{
	m_implementation->PixelFormat(redBits, greenBits, blueBits, alphaBits);
	return *this;
}

WindowCreationParams& WindowCreationParams::DepthBits(uint32_t bits)
{
	m_implementation->DepthBits(bits);
	return *this;
}

WindowCreationParams& WindowCreationParams::StencilBits(uint32_t bits)
{
	m_implementation->StencilBits(bits);
	return *this;
}

WindowCreationParams& WindowCreationParams::AntialiasSamples(uint32_t samples)
{
	m_implementation->AntialiasSamples(samples);
	return *this;
}

WindowCreationParams& WindowCreationParams::DoubleBuffer(bool doubleBuffer)
{
	m_implementation->DoubleBuffer(doubleBuffer);
	return *this;
}

WindowCreationParams& WindowCreationParams::ContextVersion(uint32_t major, uint32_t minor)
{
	m_implementation->ContextVersion(major, minor);
	return *this;
}

WindowCreationParams& WindowCreationParams::DebugContext(bool debug)
{
	m_implementation->DebugContext(debug);
	return *this;
}

WindowCreationParams& WindowCreationParams::Title(const std::string& title)
{
	m_implementation->Title(title);
	return *this;
}

uint32_t WindowCreationParams::GetWidth() const { return m_implementation->GetWidth(); }

uint32_t WindowCreationParams::GetHeight() const { return m_implementation->GetHeight(); }

uint32_t WindowCreationParams::GetXPos() const { return m_implementation->GetXPos(); }

uint32_t WindowCreationParams::GetYPos() const { return m_implementation->GetYPos(); }

bool WindowCreationParams::GetFullScreen() const { return m_implementation->GetFullScreen(); }

const uint32_t* const WindowCreationParams::GetPixelFormat() const { return m_implementation->GetPixelFormat(); }

uint32_t WindowCreationParams::GetDepthBits() const { return m_implementation->GetDepthBits(); }

uint32_t WindowCreationParams::GetStencilBits() const { return m_implementation->GetStencilBits(); }

uint32_t WindowCreationParams::GetAntialiasSamples() const { return m_implementation->GetAntialiasSamples(); }

bool WindowCreationParams::GetDoubleBuffer() const { return m_implementation->GetDoubleBuffer(); }

const uint32_t* const WindowCreationParams::GetContextVersion() const { return m_implementation->GetContextVersion(); }

bool WindowCreationParams::GetDebugContext() const { return m_implementation->GetDebugContext(); }

const std::string& WindowCreationParams::GetTitle() const { return m_implementation->GetTitle(); }

}  // namespace pgeditor::window

