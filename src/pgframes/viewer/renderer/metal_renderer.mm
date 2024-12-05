#include "metal_renderer.h"

#include "pagoda/scene/render_task.h"
#include "pagoda/scene/render_target.h"
#include "pagoda/scene/camera.h"

#include "pagoda/common/debug/assertions.h"
#include "pagoda/common/debug/logger.h"

#include <Metal/Metal.h>
#include <MetalKit/MetalKit.h>

namespace pagoda::renderer
{

static const std::string shader = R"(
#include <metal_stdlib>
using namespace metal;

vertex float4
vertexShader(uint vertexID [[vertex_id]],
             constant simd::float3* vertexPositions)
{
    float4 vertexOutPositions = float4(vertexPositions[vertexID][0],
                                       vertexPositions[vertexID][1],
                                       vertexPositions[vertexID][2],
                                       1.0f);
    return vertexOutPositions;
}

fragment float4 fragmentShader(float4 vertexOutPositions [[stage_in]]) {
    return float4(182.0f/255.0f, 240.0f/255.0f, 228.0f/255.0f, 1.0f);
}
)";

static const MTLPixelFormat metalPixelFormats[] = {
  MTLPixelFormatBGRA8Unorm
};

struct MetalRenderTarget
{
  id<MTLTexture> colorAttachmentTexture;
};

class MetalRenderer::Impl {
public:
  Impl() {
    m_camera.SetPosition({1,1,1});
    m_camera.SetViewDirection({-1, -1, -1});
  }

  ~Impl() {}

  id<MTLDevice> m_device{nullptr};
  id<MTLCommandQueue> m_commandQueue{nullptr};
  id<MTLCommandBuffer> m_commandBuffer{nullptr};
  MTLRenderPassDescriptor *m_renderPassDescriptor{nullptr};
  id<MTLRenderCommandEncoder> m_renderEncoder{nullptr};

  id<MTLLibrary> m_shaderLibrary;
  id<MTLBuffer> m_triangleBuffer;
  id<MTLRenderPipelineState> m_renderPipeline;

  pagoda::scene::Camera m_camera;

  scene::RenderTargetPtr m_renderTarget;
};

MetalRenderer::MetalRenderer() : m_impl{std::make_unique<Impl>()} {}
MetalRenderer::~MetalRenderer() {}

void MetalRenderer::InitRenderer() {
  if (m_impl->m_device != nullptr) {
    return;
  }

  m_impl->m_device = MTLCreateSystemDefaultDevice();
  if (m_impl->m_device == nullptr) {
    LOG_ERROR("Unable to create a Metal device");
    return;
  }
  
  m_impl->m_commandQueue = [m_impl->m_device newCommandQueue];

  // Create a shader
  NSString* source = [NSString stringWithCString: shader.c_str()
                                        encoding:[NSString defaultCStringEncoding]];

  NSError* error = nil;
  m_impl->m_shaderLibrary = [m_impl->m_device newLibraryWithSource:source 
                                                           options:nil 
                                                             error: &error];

 if (error != nil) {
    LOG_FATAL("Failed to compile MTLLibrary");
    LOG_FATAL([error.description UTF8String]);
    // return nil;
  }

  // Create a triangl
  simd::float3 triangleVertices[] = {
    {-0.5f, -0.5f, 0.0f},
    { 0.5f, -0.5f, 0.0f},
    { 0.0f,  0.5f, 0.0f}
  };
  m_impl->m_triangleBuffer = [m_impl->m_device newBufferWithBytes:triangleVertices 
                                                           length:sizeof(triangleVertices)
                                                          options:MTLResourceOptionCPUCacheModeDefault];

  // Create a render Pipeline
  id<MTLFunction> vertexFunction =
      [m_impl->m_shaderLibrary newFunctionWithName:@"vertexShader"];
  id<MTLFunction> fragmentFunction =
      [m_impl->m_shaderLibrary newFunctionWithName:@"fragmentShader"];

  MTLRenderPipelineDescriptor *pipelineDescriptor =
      [[MTLRenderPipelineDescriptor alloc] init];
  pipelineDescriptor.label =
      [NSString stringWithCString:"Pipeline Descriptor"
                         encoding:[NSString defaultCStringEncoding]];
  pipelineDescriptor.vertexFunction = vertexFunction;
  pipelineDescriptor.fragmentFunction = fragmentFunction;
  pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;

  m_impl->m_renderPipeline =
      [m_impl->m_device newRenderPipelineStateWithDescriptor:pipelineDescriptor
                                                       error:&error];

  [pipelineDescriptor release];

  if (error != nil) {
    LOG_FATAL("Failed to create MTLRenderPipelineState.");
    LOG_FATAL([error.description UTF8String]);
    // return nil;
  }
}

void MetalRenderer::StartFrame() {
  // Create a command buffer for the frame
  m_impl->m_commandBuffer = [m_impl->m_commandQueue commandBuffer];
  m_impl->m_commandBuffer.label = @"CommandBuffer";
}

void MetalRenderer::EndFrame() {
  DBG_ASSERT_MSG(m_impl->m_commandBuffer != nullptr,
                 "Command buffer should have been initialised");
  [m_impl->m_commandBuffer commit];
}

void MetalRenderer::StartRenderPass() {
  DBG_ASSERT_MSG(m_impl->m_commandBuffer != nullptr,
                 "Command buffer should have been initialised");

  // Set up the render pass
  auto renderTarget = m_impl->m_renderTarget;
  auto metalRenderTarget =
    reinterpret_cast<MetalRenderTarget*>(renderTarget->GetHandle());

  m_impl->m_renderPassDescriptor = [MTLRenderPassDescriptor renderPassDescriptor];
  m_impl->m_renderPassDescriptor.colorAttachments[0].texture =
      metalRenderTarget->colorAttachmentTexture;
  m_impl->m_renderPassDescriptor.colorAttachments[0].loadAction =
      MTLLoadActionClear;
  const auto clearColor = renderTarget->GetColorAttachment().clearColor;
  m_impl->m_renderPassDescriptor.colorAttachments[0].clearColor =
      MTLClearColorMake(X(clearColor), Y(clearColor), Z(clearColor), W(clearColor));
  m_impl->m_renderPassDescriptor.colorAttachments[0].storeAction =
      MTLStoreActionStore;

  // Create a render command encoder
  m_impl->m_renderEncoder = [m_impl->m_commandBuffer
      renderCommandEncoderWithDescriptor: m_impl->m_renderPassDescriptor];
  m_impl->m_renderEncoder.label = @"RenderEncoder";

  // Set the viewport
  const MTLViewport viewPort{
    0.0,
    0.0,
    static_cast<float>(renderTarget->GetWidth()),
    static_cast<float>(renderTarget->GetHeight()),
    -1.0,
    1.0};
  [m_impl->m_renderEncoder setViewport:viewPort];

  // Draw the triangle

  [m_impl->m_renderEncoder setRenderPipelineState: m_impl->m_renderPipeline];
  [m_impl->m_renderEncoder setVertexBuffer:m_impl->m_triangleBuffer offset:0 atIndex:0];
  [m_impl->m_renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
}

void MetalRenderer::EndRenderPass() {
  DBG_ASSERT_MSG(m_impl->m_renderEncoder != nullptr,
                 "Render encoder should have been initialized");
  [m_impl->m_renderEncoder endEncoding];
  m_impl->m_renderEncoder = nullptr;
  m_impl->m_renderPassDescriptor = nullptr;
}

void MetalRenderer::SetUpRenderTarget(scene::RenderTargetPtr renderTarget) {
  if (renderTarget->GetHandle() == nullptr) {
    MetalRenderTarget* metalRenderTarget = new MetalRenderTarget;
    renderTarget->SetHandle(
        reinterpret_cast<scene::RenderTarget::RenderTargetHandle*>(metalRenderTarget));

    scene::RenderTarget::Attachment& colorAttachment = renderTarget->GetColorAttachment();
    const auto pixelFormatIndex = static_cast<uint32_t>(colorAttachment.pixelFormat);
    const auto pixelFormat = metalPixelFormats[pixelFormatIndex];

    MTLTextureDescriptor *textureDescriptor = [[MTLTextureDescriptor alloc] init];
    textureDescriptor.pixelFormat = pixelFormat;
    textureDescriptor.width = renderTarget->GetWidth();
    textureDescriptor.height = renderTarget->GetHeight();
    textureDescriptor.depth = renderTarget->GetDepth();

    metalRenderTarget->colorAttachmentTexture =
      [m_impl->m_device newTextureWithDescriptor: textureDescriptor];

    colorAttachment.handle =
      reinterpret_cast<scene::RenderTarget::AttachmentHandle*>(
          metalRenderTarget->colorAttachmentTexture);
  }
  m_impl->m_renderTarget = renderTarget;
}

}
