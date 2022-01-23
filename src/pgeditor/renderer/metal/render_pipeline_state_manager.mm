#include "render_pipeline_state_manager.h"

#include "shader_gen.h"

#include <pgeditor/renderer/material_network.h>

#include <pagoda/common/debug/logger.h>

#include <ios>

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
  const std::string shaderSource = shaderGen.Generate();

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
      [library newFunctionWithName:@"vertexShader"];
  id<MTLFunction> fragmentFunction =
      [library newFunctionWithName:@"fragmentShader"];

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
  const auto &bufferRequests = shaderGen.GetBufferRequests();
  std::vector<VertexAttributeDescription> vertexAttributeDescription;
  for (const auto &b : bufferRequests) {
    const auto desc = TypeDescription::Description(b.type);
    vertexAttributeDescription.push_back(VertexAttributeDescription{
        b.name, b.semantics, b.type, desc.componentsPerElement,
        desc.bytesPerComponent});
  }
  pipelineState.vertexAttributes = vertexAttributeDescription;

  ////////////////////////////////////////
  // Create the buffer requests
  ////////////////////////////////////////
  const auto &uniformRequests = shaderGen.GetUniformRequests();
  for (const auto &r : uniformRequests) {
    pipelineState.uniforms.emplace_back(r.name, r.type);
  }

  auto i = m_renderPipelineStates.emplace(materialHash, pipelineState);
  return i.first->second;
}

} // namespace pgeditor::renderer::metal
