#pragma once

#include <MetalKit/MetalKit.h>

#include <memory>
#include <unordered_map>

namespace pgeditor::renderer
{
  class MaterialNetwork;
}

namespace pgeditor::renderer::metal
{
class RenderPipelineStateManager
{
	public:
  RenderPipelineStateManager(id<MTLDevice> device, MTLPixelFormat pixelFormat);

  id<MTLRenderPipelineState> GetRenderPipelineState(const std::shared_ptr<MaterialNetwork>& materialNetwork);

	private:
  std::unordered_map<std::size_t, id<MTLRenderPipelineState>> m_renderPipelineStates;

  id<MTLDevice> m_device;
  MTLPixelFormat m_pixelFormat; // TODO: Doesn't belong here
};

}
