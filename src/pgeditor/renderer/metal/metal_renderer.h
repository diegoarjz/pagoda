#pragma once

#include <pgeditor/renderer/collection.h>

#include <memory>

namespace pgeditor::renderer::metal
{
class MetalRenderer
{
	public:
	MetalRenderer(void* CAMetalLayerHandle);
	~MetalRenderer();

	void InitRenderer();

	void Draw(const Collection& c);

	private:
	class Impl;
	std::unique_ptr<Impl> m_impl;
};
}  // namespace pgeditor::renderer::metal
