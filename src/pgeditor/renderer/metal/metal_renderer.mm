#include "metal_renderer.h"

#include "render_pipeline_state.h"
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
class RenderPipelineStateManager
{
	public:
  RenderPipelineStateManager(id<MTLDevice> device, MTLPixelFormat pixelFormat)
    : m_device(device)
    , m_pixelFormat(pixelFormat)
  {
  }

	RenderPipelineStatePtr Create(const RenderPipelineStateDescriptor& desc)
  {
    auto it = m_pipelines.find(desc.m_materialName);
    if (it != m_pipelines.end()) {
      return it->second;
    }
    auto r = m_pipelines.emplace(desc.m_materialName, std::make_shared<RenderPipelineSate>());
    return r.first->second;
  }

	id<MTLRenderPipelineState> GetMetalPipelineState(const RenderPipelineStatePtr& state)
  {
    auto it = m_metalRenderPipelineState.find(state);
    if (it != m_metalRenderPipelineState.end()) {
      return it->second;
    }

    NSError *error = nil;
    // Create a default library and pipeline state

    MaterialNetwork network("default");
    // The Vert shader network
    auto defaultVert = network.CreateMaterialNode("defaultVert", "defaultVert");
    defaultVert->SetInput("position", {"position", Type::Vec4});
    defaultVert->SetInput("viewport", {"viewport", Type::Vec2});
    defaultVert->SetOutput("position", {"position", Type::Vec4});
    network.SetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex, "defaultVert");

    auto positionNode = network.CreateMaterialNode("bufferView", "position");
    positionNode->SetOutput("position", {"position", Type::Vec4});
    positionNode->SetParameter("bufferName", "position");
    positionNode->SetParameter("semantics", static_cast<int>(VertexAttributeSemantics::Position));
    positionNode->SetParameter("type", static_cast<int>(Type::Vec4));

    auto viewPortUniformNode = network.CreateMaterialNode("uniformView", "viewport");
    viewPortUniformNode->SetOutput("viewport", {"viewport", Type::UInt2});
    viewPortUniformNode->SetParameter("uniformName", "viewport");
    viewPortUniformNode->SetParameter("type", static_cast<int>(Type::UInt2));

    defaultVert->GetInput("position")->m_upstreamNode = "position";
    defaultVert->GetInput("position")->m_upstreamOutput = "position";
    defaultVert->GetInput("viewport")->m_upstreamNode = "viewport";
    defaultVert->GetInput("viewport")->m_upstreamOutput = "viewport";

    // The frag shader network
    auto defaultFrag = network.CreateMaterialNode("defaultFrag", "defaultFrag");
    defaultFrag->SetInput("color", {"color", Type::Vec4});
    defaultFrag->SetOutput("color", {"color", Type::Vec4});

    auto colorNode = network.CreateMaterialNode("bufferView", "color");
    colorNode->SetOutput("color", {"color", Type::Vec4});
    colorNode->SetParameter("bufferName", "color");
    colorNode->SetParameter("semantics", static_cast<int>(VertexAttributeSemantics::Color));
    colorNode->SetParameter("type", static_cast<int>(Type::Vec4));

    defaultFrag->GetInput("color")->m_upstreamNode = "color";
    defaultFrag->GetInput("color")->m_upstreamOutput = "color";

    network.SetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment, "defaultFrag");
    auto defaultShaderSource = ShaderGen{network}.Generate();

    std::cout << "Shader source" << std::endl;
    std::cout << defaultShaderSource << std::endl;

    NSString *source = [NSString stringWithCString:defaultShaderSource.c_str()
                                          encoding:[NSString defaultCStringEncoding]];
    id<MTLLibrary> library = [m_device newLibraryWithSource: source
                                                    options: nil
                                                      error: &error];

    if (error != nil) {
      LOG_FATAL("Failed to compile default pipeline shader");
      LOG_FATAL([error.description UTF8String]);
    }

    id<MTLFunction> vertexFunction = [library newFunctionWithName: @"vertexShader"];
    id<MTLFunction> fragmentFunction = [library newFunctionWithName: @"fragmentShader"];
    MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.label = [NSString stringWithCString:state->GetDebugName().c_str()
                                                  encoding:[NSString defaultCStringEncoding]];
    pipelineDescriptor.vertexFunction = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.colorAttachments[0].pixelFormat = m_pixelFormat;

    id<MTLRenderPipelineState> pipeline = [m_device newRenderPipelineStateWithDescriptor: pipelineDescriptor
                                                                                   error: &error];

    if (error != nil) {
      LOG_FATAL("Failed to create default render pipeline");
      LOG_FATAL([error.description UTF8String]);
    }

    auto i = m_metalRenderPipelineState.emplace(state, pipeline);
    return i.first->second;
  }

	private:
	// TODO: this should not be a string
	std::unordered_map<std::string, RenderPipelineStatePtr> m_pipelines;
  std::unordered_map<RenderPipelineStatePtr, id<MTLRenderPipelineState>> m_metalRenderPipelineState;

  id<MTLDevice> m_device;
  MTLPixelFormat m_pixelFormat; // TODO Doesn't belong here
};

class MetalRenderer::Impl
{
	public:
  Impl(void* handle) : m_layer{static_cast<CAMetalLayer*>(handle)}
  {
  }

  void CreateDefaultPipelineState()
  {
    if (m_pipelineManager == nullptr) {
      m_pipelineManager = std::make_shared<RenderPipelineStateManager>(m_device, m_layer.pixelFormat);
    }

    RenderPipelineStateDescriptor desc;
    desc.m_debugName = "Default Pipeline Descriptor";
    desc.m_materialName = "Default Material";

    auto pipeline = m_pipelineManager->Create(desc);
    m_defaultPipeline = m_pipelineManager->GetMetalPipelineState(pipeline);
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

  m_impl->CreateDefaultPipelineState();

  m_impl->m_commandQueue = [m_impl->m_device newCommandQueue];
}

void MetalRenderer::Draw(const Collection& collection)
{
  auto commandQueue = m_impl->m_commandQueue;
  auto layer = m_impl->m_layer;
  auto defaultPipeline = m_impl->m_defaultPipeline;

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
    [renderEncoder setRenderPipelineState: defaultPipeline];

    for (const auto& r : collection) {
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
