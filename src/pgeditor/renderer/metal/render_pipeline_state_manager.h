#pragma once

#include <pgeditor/renderer/vertex_attribute.h>

#include <pagoda/common/types.h>

#include <MetalKit/MetalKit.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace pagoda::material {
class MaterialNetwork;
}

namespace pgeditor::renderer::metal {

struct RenderPipelineState {
  std::vector<VertexAttributeDescription> vertexAttributes;
  std::vector<std::pair<std::string, pagoda::common::Type>> uniforms;
  id<MTLRenderPipelineState> pipelineState;
};

class RenderPipelineStateManager {
public:
  RenderPipelineStateManager(id<MTLDevice> device, MTLPixelFormat pixelFormat);

  const RenderPipelineState &GetRenderPipelineState(
      const std::shared_ptr<pagoda::material::MaterialNetwork> &materialNetwork);

private:
  std::unordered_map<std::size_t, RenderPipelineState> m_renderPipelineStates;

  id<MTLDevice> m_device;
  MTLPixelFormat m_pixelFormat; // TODO: Doesn't belong here
};

} // namespace pgeditor::renderer::metal
