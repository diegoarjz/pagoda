#include "metal_renderer.h"

#include "render_pipeline_state.h"
#include "render_pipeline_state_manager.h"
#include "shader_gen.h"

#include <pgeditor/renderer/renderable.h>
#include <pgeditor/renderer/material_network.h>
#include <pgeditor/renderer/material_node.h>
#include <pgeditor/renderer/interleaved_buffer.h>
#include <pgeditor/renderer/vertex_attribute.h>

#include <pagoda/common/debug/logger.h>

#include <MetalKit/MetalKit.h>

#include <unordered_map>

using namespace pagoda::math;

namespace pgeditor::renderer::metal
{
class MetalRenderer::Impl
{
	public:
  Impl(void* handle) : m_layer{static_cast<CAMetalLayer*>(handle)}
  {
  }

  CAMetalLayer* m_layer;
  id<MTLDevice> m_device{nullptr};
  id<MTLRenderPipelineState> m_defaultPipeline; // TODO: This doesn't belong here
  id<MTLCommandQueue> m_commandQueue;
  std::shared_ptr<RenderPipelineStateManager> m_pipelineManager;
};

MetalRenderer::MetalRenderer(void* handle) : m_impl{std::make_unique<Impl>(handle)}
{
}

MetalRenderer::~MetalRenderer()
{
}

void MetalRenderer::InitRenderer()
{
  if (m_impl->m_device) {
    LOG_WARNING("Metal device already initialized");
    return;
  }

  m_impl->m_layer.device = MTLCreateSystemDefaultDevice();
  m_impl->m_device = m_impl->m_layer.device;
  if (m_impl->m_device == nullptr) {
    LOG_FATAL("Unable to create a Metal device");
    return;
  }

  m_impl->m_pipelineManager = std::make_shared<RenderPipelineStateManager>(m_impl->m_device, m_impl->m_layer.pixelFormat);
  m_impl->m_commandQueue = [m_impl->m_device newCommandQueue];
}

void MetalRenderer::Draw(const Collection& collection)
{
  auto commandQueue = m_impl->m_commandQueue;
  auto layer = m_impl->m_layer;

  id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
  commandBuffer.label = @"MyCommand";

  @autoreleasepool {
    // Obtain a renderPassDescriptor generated from the view's drawable textures
    id<CAMetalDrawable> drawable = [layer nextDrawable];
    if (drawable == nil) {
      return;
    }

    MTLRenderPassDescriptor* renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
    renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
    renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

    // Create a render encoder for the render pass
    id<MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderPassDescriptor];
    renderEncoder.label = @"MyRenderEncoder";

    // Set the viewport
    [renderEncoder setViewport:(MTLViewport){0.0, 0.0, layer.drawableSize.width, layer.drawableSize.height, -1.0, 1.0}];

    for (const auto& r : collection) {
      auto materialNetwork = r->GetMaterial();
      id<MTLRenderPipelineState> pipeline = m_impl->m_pipelineManager->GetRenderPipelineState(materialNetwork);
      [renderEncoder setRenderPipelineState: pipeline];

      std::vector<const Buffer*> buffers{
        &r->GetBuffer("position"), &r->GetBuffer("color")
      };
      std::vector<VertexAttributeDescription> vertexDescription = {
        {VertexAttributeSemantics::Position, Type::Vec4, 4, sizeof(float)},
        {VertexAttributeSemantics::Color, Type::Vec4, 4, sizeof(float)},
      };
      InterleavedBuffer buffer(vertexDescription, buffers);

      [renderEncoder setVertexBytes: buffer.GetData()
                             length: buffer.GetSize()
                            atIndex: 0];

      vector_uint2 viewportSize = { (unsigned int)(layer.bounds.size.width),
                                    (unsigned int)(layer.bounds.size.height) };

      [renderEncoder setVertexBytes: &viewportSize
                             length: sizeof(viewportSize)
                            atIndex: 1];

      // Draw the 3 vertices
      [renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle
                        vertexStart: 0
                        vertexCount: r->GetVertexCount()];
    }

    [renderEncoder endEncoding];

    [commandBuffer presentDrawable: drawable];
    [commandBuffer commit];
  }
}

}  // namespace pgeditor::gui::renderer::metal
