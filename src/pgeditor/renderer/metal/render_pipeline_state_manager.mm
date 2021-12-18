#include "render_pipeline_state_manager.h"

#include "shader_gen.h"

#include <pgeditor/renderer/material_network.h>

#include <pagoda/common/debug/logger.h>

#include <ios>

namespace pgeditor::renderer::metal
{
RenderPipelineStateManager::RenderPipelineStateManager(id<MTLDevice> device, MTLPixelFormat pixelFormat)
  : m_device(device)
  , m_pixelFormat(pixelFormat)
{
}

id<MTLRenderPipelineState> RenderPipelineStateManager::GetRenderPipelineState(const std::shared_ptr<MaterialNetwork>& materialNetwork)
{
  std::size_t materialHash = 0;
  materialNetwork->AppendHash(materialHash);

  auto iter = m_renderPipelineStates.find(materialHash);
  if (iter != m_renderPipelineStates.end()) {
    return iter->second;
  }

  LOG_DEBUG("Creating a RenderPipelineState.");
  LOG_DEBUG("  Material: " << materialNetwork->GetName() << " (0x" << std::hex << materialHash << ")");

  // We didn't find a render pipeline state for the hash, so we need to create it
  NSError* error = nil;

  // Start by generating the shader code
  ShaderGen shaderGen{*materialNetwork};
  const std::string shaderSource = shaderGen.Generate();

  NSString* source = [NSString stringWithCString: shaderSource.c_str()
                                        encoding: [NSString defaultCStringEncoding]];

  id<MTLLibrary> library = [m_device newLibraryWithSource: source
                                                  options: nil
                                                    error: &error];

  if (error != nil) {
    LOG_FATAL("Failed to compile MTLLibrary for MaterialNetwork: " << materialNetwork->GetName() << ".");
    LOG_FATAL([error.description UTF8String]);
    return nil;
  }

  id<MTLFunction> vertexFunction = [library newFunctionWithName: @"vertexShader"];
  id<MTLFunction> fragmentFunction = [library newFunctionWithName: @"fragmentShader"];

  // Now create the RenderPipeline
  MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
  pipelineDescriptor.label = [NSString stringWithCString: materialNetwork->GetName().c_str()
                                                encoding: [NSString defaultCStringEncoding]];
  pipelineDescriptor.vertexFunction = vertexFunction;
  pipelineDescriptor.fragmentFunction = fragmentFunction;
  pipelineDescriptor.colorAttachments[0].pixelFormat = m_pixelFormat;

  id<MTLRenderPipelineState> pipeline = [m_device newRenderPipelineStateWithDescriptor: pipelineDescriptor
                                                                                 error: &error];

  if (error != nil) {
    LOG_FATAL("Failed to create MTLRenderPipelineState.");
    LOG_FATAL([error.description UTF8String]);
    return nil;
  }

  auto i = m_renderPipelineStates.emplace(materialHash, pipeline);
  return i.first->second;
}

}
