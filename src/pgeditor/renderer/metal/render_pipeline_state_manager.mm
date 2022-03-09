#include "render_pipeline_state_manager.h"

#include <pagoda/material/material_network.h>
#include <pagoda/material/material_node.h>
#include <pagoda/material/shader_gen.h>

#include <pagoda/common/debug/logger.h>

#include <ios>

using namespace pagoda::common;
using namespace pagoda::material;

namespace pgeditor::renderer::metal {
RenderPipelineStateManager::RenderPipelineStateManager(
    id<MTLDevice> device, MTLPixelFormat pixelFormat)
    : m_device(device), m_pixelFormat(pixelFormat) {}

const RenderPipelineState &RenderPipelineStateManager::GetRenderPipelineState(
    const std::shared_ptr<MaterialNetwork> &materialNetwork) {
  std::size_t materialHash = 0;
  materialNetwork->AppendHash(materialHash);

  auto iter = m_renderPipelineStates.find(materialHash);
  if (iter != m_renderPipelineStates.end()) {
    return iter->second;
  }

  LOG_DEBUG("Creating a RenderPipelineState.");
  LOG_DEBUG("  Material: " << materialNetwork->GetName() << " (0x" << std::hex
                           << materialHash << ")");

  // We didn't find a render pipeline state for the hash, so we need to create
  // it
  NSError *error = nil;

  // Start by generating the shader code
  ShaderGen shaderGen{*materialNetwork};
  std::string shaderSource = R"(#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
)";
  for (const auto &stage : {MaterialNetwork::ShaderStage::Vertex,
                            MaterialNetwork::ShaderStage::Fragment}) {
    std::stringstream stageSource;
    auto source =
        ShaderSource::Create(ShaderSource::ShadingLanguage::MSL, stageSource);
    auto shaderStage = shaderGen.GetStage(stage);
    shaderStage->GetSource(*source);

    shaderSource += stageSource.str() + "\n\n";
  }

  std::cout << "----------------------------------------" << std::endl;
  std::cout << shaderSource << "\n\n" << std::endl;

  ////////////////////////////////////////
  // Create the library
  ////////////////////////////////////////
  NSString *source =
      [NSString stringWithCString:shaderSource.c_str()
                         encoding:[NSString defaultCStringEncoding]];

  id<MTLLibrary> library =
      [m_device newLibraryWithSource:source options:nil error:&error];

  if (error != nil) {
    LOG_FATAL("Failed to compile MTLLibrary for MaterialNetwork: "
              << materialNetwork->GetName() << ".");
    LOG_FATAL([error.description UTF8String]);
    // return nil;
  }

  id<MTLFunction> vertexFunction =
      [library newFunctionWithName:@"main_vertex"];
  id<MTLFunction> fragmentFunction =
      [library newFunctionWithName:@"main_fragment"];

  ////////////////////////////////////////
  // Now create the RenderPipeline
  ////////////////////////////////////////
  MTLRenderPipelineDescriptor *pipelineDescriptor =
      [[MTLRenderPipelineDescriptor alloc] init];
  pipelineDescriptor.label =
      [NSString stringWithCString:materialNetwork->GetName().c_str()
                         encoding:[NSString defaultCStringEncoding]];
  pipelineDescriptor.vertexFunction = vertexFunction;
  pipelineDescriptor.fragmentFunction = fragmentFunction;
  pipelineDescriptor.colorAttachments[0].pixelFormat = m_pixelFormat;

  // Set the depth state pixelFormat
  pipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;

  id<MTLRenderPipelineState> pipeline =
      [m_device newRenderPipelineStateWithDescriptor:pipelineDescriptor
                                               error:&error];

  if (error != nil) {
    LOG_FATAL("Failed to create MTLRenderPipelineState.");
    LOG_FATAL([error.description UTF8String]);
    // return nil;
  }

  RenderPipelineState pipelineState;
  pipelineState.pipelineState = pipeline;

  ////////////////////////////////////////
  // Create the vertex attribute description
  ////////////////////////////////////////
  auto vertStage = shaderGen.GetStage(MaterialNetwork::ShaderStage::Vertex);
  const auto &bufferRequests = vertStage->GetStageInputRequests();
  std::vector<VertexAttributeDescription> vertexAttributeDescription;
  for (const auto &b : bufferRequests) {
    const auto desc = TypeDescription::Description(b.type);
    vertexAttributeDescription.push_back(VertexAttributeDescription{
        b.name, b.type, desc.componentsPerElement, desc.bytesPerComponent});
  }
  pipelineState.vertexAttributes = vertexAttributeDescription;

  ////////////////////////////////////////
  // Create the buffer requests
  ////////////////////////////////////////
  const auto &vertUniforms = vertStage->GetUniformRequests();
  for (const auto &r : vertUniforms) {
    pipelineState.vertexUniforms.emplace_back(r.name, r.type);
  }

  auto fragStage = shaderGen.GetStage(MaterialNetwork::ShaderStage::Fragment);
  const auto& fragUniforms = fragStage->GetUniformRequests();
  for (const auto &r : fragUniforms) {
    pipelineState.fragmentUniforms.emplace_back(r.name, r.type);
  }


  auto i = m_renderPipelineStates.emplace(materialHash, pipelineState);
  return i.first->second;
}

} // namespace pgeditor::renderer::metal
