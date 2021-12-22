#include "metal_renderer.h"

#include "render_pipeline_state.h"
#include "render_pipeline_state_manager.h"
#include "shader_gen.h"

#include <pgeditor/renderer/interleaved_buffer.h>
#include <pgeditor/renderer/material_network.h>
#include <pgeditor/renderer/material_node.h>
#include <pgeditor/renderer/renderable.h>
#include <pgeditor/renderer/uniform_buffer.h>
#include <pgeditor/renderer/vertex_attribute.h>

#include <pagoda/math/matrix_base.h>
#include <pagoda/scene/camera.h>

#include <pagoda/common/debug/logger.h>

#include <MetalKit/MetalKit.h>

#include <unordered_map>

using namespace pagoda::math;
using namespace pagoda::scene;

namespace pgeditor::renderer::metal {
class MetalRenderer::Impl {
public:
  Impl(void *handle) : m_layer{static_cast<CAMetalLayer *>(handle)} {}

  CAMetalLayer *m_layer;
  id<MTLDevice> m_device{nullptr};
  id<MTLRenderPipelineState>
      m_defaultPipeline; // TODO: This doesn't belong here
  id<MTLCommandQueue> m_commandQueue;
  std::shared_ptr<RenderPipelineStateManager> m_pipelineManager;

  Camera m_camera;
};

MetalRenderer::MetalRenderer(void *handle)
    : m_impl{std::make_unique<Impl>(handle)} {}

MetalRenderer::~MetalRenderer() {}

void MetalRenderer::InitRenderer() {
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

  m_impl->m_pipelineManager = std::make_shared<RenderPipelineStateManager>(
      m_impl->m_device, m_impl->m_layer.pixelFormat);
  m_impl->m_commandQueue = [m_impl->m_device newCommandQueue];
}

void MetalRenderer::Draw(const Collection &collection) {
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

    MTLRenderPassDescriptor *renderPassDescriptor =
        [MTLRenderPassDescriptor renderPassDescriptor];
    renderPassDescriptor.colorAttachments[0].texture = drawable.texture;
    renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    renderPassDescriptor.colorAttachments[0].clearColor =
        MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
    renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

    // Create a render encoder for the render pass
    id<MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    renderEncoder.label = @"MyRenderEncoder";

    // Set the viewport
    [renderEncoder
        setViewport:(MTLViewport){0.0, 0.0, layer.drawableSize.width,
                                  layer.drawableSize.height, -1.0, 1.0}];

    for (const auto &r : collection) {
      auto materialNetwork = r->GetMaterial();
      auto pipelineState =
          m_impl->m_pipelineManager->GetRenderPipelineState(materialNetwork);
      id<MTLRenderPipelineState> pipeline = pipelineState.pipelineState;
      [renderEncoder setRenderPipelineState:pipeline];

      ////////////////////////////////////////
      // Prepare the uniform buffer
      ////////////////////////////////////////
      UniformBuffer uniforms;
      for (const auto &uniform : pipelineState.uniforms) {
        const auto &uniformName = std::get<0>(uniform);
        if (uniformName == "modelMatrix") {
          uniforms.Add<Mat4x4F>(boost::qvm::transposed(r->GetModelMatrix()));
        } else if (uniformName == "viewMatrix") {
          uniforms.Add<Mat4x4F>(
              boost::qvm::transposed(m_impl->m_camera.GetViewMatrix()));
        } else if (uniformName == "projectionMatrix") {
          uniforms.Add<Mat4x4F>(
              boost::qvm::transposed(m_impl->m_camera.GetProjectionMatrix()));
        }
      }

      ////////////////////////////////////////
      // Prepare the vertex buffer
      ////////////////////////////////////////
      const auto &vertexAttributeDescription = pipelineState.vertexAttributes;
      std::vector<const Buffer *> buffers;
      for (const auto &attr : vertexAttributeDescription) {
        buffers.push_back(r->GetBuffer(attr.name));
      }
      InterleavedBuffer buffer(vertexAttributeDescription, buffers);

      [renderEncoder setVertexBytes:buffer.GetData()
                             length:buffer.GetSize()
                            atIndex:0];

      [renderEncoder setVertexBytes:uniforms.GetData()
                             length:uniforms.GetSize()
                            atIndex:1];

      [renderEncoder setFragmentBytes:uniforms.GetData()
                               length:uniforms.GetSize()
                              atIndex:0];

      [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                        vertexStart:0
                        vertexCount:r->GetVertexCount()];
    }

    [renderEncoder endEncoding];

    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
  }
}

void MetalRenderer::SetCamera(pagoda::scene::Camera &cam) {
  m_impl->m_camera = cam;
}

void MetalRenderer::SetDisplaySize(const pagoda::math::Vec2U &size) {
  if (m_impl->m_layer) {
    CGSize cgSize;
    cgSize.width = X(size);
    cgSize.height = Y(size);
    [m_impl->m_layer setDrawableSize:cgSize];
  }
}
} // namespace pgeditor::renderer::metal
