#include "metal_renderer.h"

#include "metal_texture.h"
#include "render_pipeline_state.h"
#include "render_pipeline_state_manager.h"
#include "shader_gen.h"

#include <pgeditor/renderer/interleaved_buffer.h>
#include <pgeditor/renderer/renderable.h>
#include <pgeditor/renderer/uniform_buffer.h>
#include <pgeditor/renderer/vertex_attribute.h>

#include <pagoda/material/material_network.h>
#include <pagoda/material/material_node.h>

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
  id<MTLCommandQueue> m_commandQueue{nullptr};
  id<MTLCommandBuffer> m_commandBuffer{nullptr};
  id<CAMetalDrawable> m_drawable{nullptr};
  MTLRenderPassDescriptor* m_renderPassDescriptor;
  id<MTLRenderCommandEncoder> m_renderEncoder{nullptr};

  std::shared_ptr<RenderPipelineStateManager> m_pipelineManager;

  id<MTLDepthStencilState> m_depthStencilState;
  std::shared_ptr<MetalTexture> m_depthTexture{nullptr};

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
  DBG_ASSERT_MSG(m_impl->m_renderEncoder != nullptr, "Render encoder should have been initialized.");

  for (const auto &r : collection) {
    auto materialNetwork = r->GetMaterial();
    auto pipelineState =
        m_impl->m_pipelineManager->GetRenderPipelineState(materialNetwork);
    id<MTLRenderPipelineState> pipeline = pipelineState.pipelineState;
    [m_impl->m_renderEncoder setRenderPipelineState:pipeline];

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

    [m_impl->m_renderEncoder setVertexBytes:buffer.GetData()
                           length:buffer.GetSize()
                          atIndex:0];

    [m_impl->m_renderEncoder setVertexBytes:uniforms.GetData()
                           length:uniforms.GetSize()
                          atIndex:1];

    [m_impl->m_renderEncoder setFragmentBytes:uniforms.GetData()
                             length:uniforms.GetSize()
                            atIndex:0];

    [m_impl->m_renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                      vertexStart:0
                      vertexCount:r->GetVertexCount()];
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

void MetalRenderer::updateRenderState(const RenderState::Diff_t& changedState)
{
  static const MTLCompareFunction compareFunctions[] = {
    MTLCompareFunctionNever,
    MTLCompareFunctionLess,
    MTLCompareFunctionEqual,
    MTLCompareFunctionLessEqual,
    MTLCompareFunctionGreater,
    MTLCompareFunctionNotEqual,
    MTLCompareFunctionGreaterEqual,
    MTLCompareFunctionAlways
  };

  DBG_ASSERT_MSG(m_impl->m_layer != nullptr, "CAMetalLayer is null.");
  DBG_ASSERT_MSG(m_impl->m_device != nullptr, "Metal device should have been initialised.");

  ////////////////////////////////////////
  // Color Attachment
  ////////////////////////////////////////
  m_impl->m_renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
  m_impl->m_renderPassDescriptor.colorAttachments[0].texture = m_impl->m_drawable.texture;
  m_impl->m_renderPassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
  m_impl->m_renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
  m_impl->m_renderPassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

  ////////////////////////////////////////
  // Depth state
  ////////////////////////////////////////
  if (m_newRenderState.depthTestEnabled) {
    CGSize size = [m_impl->m_layer drawableSize];
    Vec2U displaySize{static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height)};

    ////////////////////////////////////////
    // (Re)Create the depth attachment
    ////////////////////////////////////////
    if (m_impl->m_depthTexture == nullptr || m_impl->m_depthTexture->GetSize() != displaySize) {
      m_impl->m_depthTexture = std::make_shared<MetalTexture>(X(displaySize),
                                                              Y(displaySize),
                                                              MetalTexture::PixelFormat::Depth32Float,
                                                              MetalTexture::Usage::RenderTarget,
                                                              MetalTexture::StorageMode::Private);
      m_impl->m_depthTexture->Create(m_impl->m_device);
    }

    ////////////////////////////////////////
    // Create the depth stencil descriptor
    ////////////////////////////////////////
    MTLDepthStencilDescriptor *depthStencilDesc = [MTLDepthStencilDescriptor new];
    depthStencilDesc.depthWriteEnabled = m_newRenderState.depthWriteEnabled;
    depthStencilDesc.depthCompareFunction = compareFunctions[static_cast<size_t>(m_newRenderState.depthFunc)];

    // TODO: Reuse the depth stencil state
    m_impl->m_depthStencilState = [m_impl->m_device newDepthStencilStateWithDescriptor:depthStencilDesc];

    ////////////////////////////////////////
    // Set the depth attachment
    ////////////////////////////////////////
    m_impl->m_renderPassDescriptor.depthAttachment.texture = m_impl->m_depthTexture->GetNativeTexture();
    m_impl->m_renderPassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    m_impl->m_renderPassDescriptor.depthAttachment.storeAction = MTLStoreActionDontCare;
    m_impl->m_renderPassDescriptor.depthAttachment.clearDepth = 1.0;
  }
}

void MetalRenderer::StartFrame()
{
  DBG_ASSERT_MSG(m_impl->m_layer, "CAMetalLayer is null");
  // Get the next drawable from the CAMetalLayer
  m_impl->m_drawable = [m_impl->m_layer nextDrawable];
  if (m_impl->m_drawable == nullptr) {
    LOG_ERROR("Drawable is not ready when starting frame.");
    return;
  }

  // Create a command buffer for the frame
  m_impl->m_commandBuffer = [m_impl->m_commandQueue commandBuffer];
  m_impl->m_commandBuffer.label = @"CommandBuffer";
}

void MetalRenderer::EndFrame()
{
  DBG_ASSERT_MSG(m_impl->m_commandBuffer != nullptr, "Command buffer should have been initialised");
  DBG_ASSERT_MSG(m_impl->m_drawable != nullptr, "Drawable should have been initialised");
  // Present the drawable and commit
  [m_impl->m_commandBuffer presentDrawable: m_impl->m_drawable];
  [m_impl->m_commandBuffer commit];
}

void MetalRenderer::StartRenderPass()
{
  DBG_ASSERT_MSG(m_impl->m_commandBuffer != nullptr, "Command buffer should have been initialised");
  DBG_ASSERT_MSG(m_impl->m_renderPassDescriptor != nullptr, "Render pass descriptor should have been initialised");
  // Create a render command encoder
  m_impl->m_renderEncoder = [m_impl->m_commandBuffer renderCommandEncoderWithDescriptor: m_impl->m_renderPassDescriptor];
  m_impl->m_renderEncoder.label = @"RenderEncoder";

  // Set the viewport
  CGSize size = [m_impl->m_layer drawableSize];
  [m_impl->m_renderEncoder
      setViewport:(MTLViewport){0.0, 0.0, size.width, size.height, -1.0, 1.0}];

  // Set its depth stencil state if enabled
  if (m_lastRenderState.depthTestEnabled) {
    DBG_ASSERT_MSG(m_impl->m_depthStencilState != nullptr, "Depth stencil state not initialied");
    [m_impl->m_renderEncoder setDepthStencilState: m_impl->m_depthStencilState];
  }
}

void MetalRenderer::EndRenderPass()
{
  DBG_ASSERT_MSG(m_impl->m_renderEncoder != nullptr, "Render encoder should have been initialized");
  [m_impl->m_renderEncoder endEncoding];
  m_impl->m_renderEncoder = nullptr;
  m_impl->m_renderPassDescriptor = nullptr;
}

} // namespace pgeditor::renderer::metal
