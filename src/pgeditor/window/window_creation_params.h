#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace pgeditor::window
{
/**
 * Parameters for creating a window.
 */
class WindowCreationParams
{
public:
	/**
	 * Creates \c WindowCreationParams with reasonable default values
	 */
	WindowCreationParams();
	~WindowCreationParams();

	WindowCreationParams& Size(uint32_t width, uint32_t height);
	WindowCreationParams& Position(uint32_t xPos, uint32_t yPos);
	WindowCreationParams& FullScreen(bool fullScreen);

	WindowCreationParams& PixelFormat(uint32_t redBits, uint32_t greenBits, uint32_t blueBits, uint32_t alphaBits);
	WindowCreationParams& DepthBits(uint32_t bits);
	WindowCreationParams& StencilBits(uint32_t bits);
	WindowCreationParams& AntialiasSamples(uint32_t samples);
	WindowCreationParams& DoubleBuffer(bool doubleBuffer);

	WindowCreationParams& ContextVersion(uint32_t major, uint32_t minor);

	WindowCreationParams& DebugContext(bool debug);
	// WindowCreationParams& Profile(ProfileType profile);

	WindowCreationParams& Title(const std::string& title);

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	uint32_t GetXPos() const;
	uint32_t GetYPos() const;
	bool GetFullScreen() const;
	const uint32_t* const GetPixelFormat() const;
	uint32_t GetDepthBits() const;
	uint32_t GetStencilBits() const;
	uint32_t GetAntialiasSamples() const;
	bool GetDoubleBuffer() const;
	const uint32_t* const GetContextVersion() const;
	bool GetDebugContext() const;
	const std::string& GetTitle() const;

private:
	struct Impl;
	std::unique_ptr<Impl> m_implementation;
};  // class WindowCreationParams
}  // namespace pgeditor::window

