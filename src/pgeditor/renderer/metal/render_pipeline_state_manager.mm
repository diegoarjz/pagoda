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
  auto material = materialNetwork;
  if (material == nullptr) {
    static std::shared_ptr<MaterialNetwork> defaultMaterialNetwork;

    if (defaultMaterialNetwork == nullptr) {
      // Create a material network
      defaultMaterialNetwork = std::make_shared<MaterialNetwork>("default");
      // The Vert shader network
      auto defaultVert = defaultMaterialNetwork->CreateMaterialNode(
          "defaultVert", "defaultVert");
      defaultVert->SetInput("position", {"position", Type::Vec4});
      defaultVert->SetInput("projectionMatrix", {"position", Type::Mat4});
      defaultVert->SetInput("viewMatrix", {"position", Type::Mat4});
      defaultVert->SetInput("modelMatrix", {"position", Type::Mat4});
      defaultVert->SetOutput("position", {"position", Type::Vec4});
      defaultMaterialNetwork->SetStageTerminalNode(
          MaterialNetwork::ShaderStage::Vertex, "defaultVert");

      auto positionNode =
          defaultMaterialNetwork->CreateMaterialNode("bufferView", "position");
      positionNode->SetOutput("position", {"position", Type::Vec4});
      positionNode->SetParameter("bufferName", "position");
      positionNode->SetParameter(
          "semantics", static_cast<int>(VertexAttributeSemantics::Position));
      positionNode->SetParameter("type", static_cast<int>(Type::Vec4));

      auto projMatrix = defaultMaterialNetwork->CreateMaterialNode(
          "uniformView", "projectionMatrix");
      projMatrix->SetOutput("projectionMatrix",
                            {"projectionMatrix", Type::Mat4});
      projMatrix->SetParameter("uniformName", "projectionMatrix");
      projMatrix->SetParameter("type", static_cast<int>(Type::Mat4));

      auto viewMatrix = defaultMaterialNetwork->CreateMaterialNode(
          "uniformView", "viewMatrix");
      viewMatrix->SetOutput("viewMatrix", {"viewMatrix", Type::Mat4});
      viewMatrix->SetParameter("uniformName", "viewMatrix");
      viewMatrix->SetParameter("type", static_cast<int>(Type::Mat4));

      auto modelMatrix = defaultMaterialNetwork->CreateMaterialNode(
          "uniformView", "modelMatrix");
      modelMatrix->SetOutput("modelMatrix", {"modelMatrix", Type::Mat4});
      modelMatrix->SetParameter("uniformName", "modelMatrix");
      modelMatrix->SetParameter("type", static_cast<int>(Type::Mat4));

      defaultVert->ConnectInput("position", positionNode, "position");
      defaultVert->ConnectInput("projectionMatrix", projMatrix,
                                "projectionMatrix");
      defaultVert->ConnectInput("viewMatrix", viewMatrix, "viewMatrix");
      defaultVert->ConnectInput("modelMatrix", modelMatrix, "modelMatrix");

      // The frag shader network
      auto defaultFrag = defaultMaterialNetwork->CreateMaterialNode(
          "defaultFrag", "defaultFrag");
      defaultFrag->SetInput("color", {"color", Type::Vec4});
      defaultFrag->SetOutput("color", {"color", Type::Vec4});

      auto colorNode =
          defaultMaterialNetwork->CreateMaterialNode("bufferView", "color");
      colorNode->SetOutput("color", {"color", Type::Vec4});
      colorNode->SetParameter("bufferName", "color");
      colorNode->SetParameter(
          "semantics", static_cast<int>(VertexAttributeSemantics::Color));
      colorNode->SetParameter("type", static_cast<int>(Type::Vec4));

      defaultFrag->ConnectInput("color", colorNode, "color");

      defaultMaterialNetwork->SetStageTerminalNode(
          MaterialNetwork::ShaderStage::Fragment, "defaultFrag");
    }

    material = defaultMaterialNetwork;
  }
  std::size_t materialHash = 0;
  material->AppendHash(materialHash);

  auto iter = m_renderPipelineStates.find(materialHash);
  if (iter != m_renderPipelineStates.end()) {
    return iter->second;
  }

  LOG_DEBUG("Creating a RenderPipelineState.");
  LOG_DEBUG("  Material: " << material->GetName() << " (0x" << std::hex
                           << materialHash << ")");

  // We didn't find a render pipeline state for the hash, so we need to create
  // it
  NSError *error = nil;

  // Start by generating the shader code
  ShaderGen shaderGen{*material};
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
              << material->GetName() << ".");
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
      [NSString stringWithCString:material->GetName().c_str()
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
