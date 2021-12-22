#include "shader_gen.h"

#include <pagoda/common/debug/logger.h>

#include <pgeditor/renderer/material_network.h>
#include <pgeditor/renderer/material_node.h>

#include <iostream>
#include <stack>

using namespace pagoda::math;

namespace pgeditor::renderer::metal {
namespace {
inline const std::string &typeName(Type t) {
  static const std::string sTypenames[] = {
      "int",    "int2",   "uint",   "uint2",    "float",    "double",
      "float2", "float3", "float4", "float2x2", "float3x3", "float4x4",
  };
  return sTypenames[static_cast<int>(t)];
}

inline const std::string &shaderTypeName(MaterialNetwork::ShaderStage stage) {
  static const std::string sNames[] = {"Vertex", "Fragment"};
  return sNames[static_cast<int>(stage)];
}

void writeStructMember(std::stringstream &ss, Type type,
                       const std::string &name,
                       VertexAttributeSemantics semantics,
                       bool withAttributes) {
  if (withAttributes && semantics == VertexAttributeSemantics::Position) {
    ss << "  " << typeName(type) << " " << name << " [[ position ]];\n";
  } else {
    ss << "  " << typeName(type) << " " << name << ";\n";
  }
}

template <class T>
void writeStruct(std::stringstream &ss, const std::string &name,
                 const T &contents) {
  ss << "struct " << name << " {\n";
  for (const auto &p : contents) {
    auto c = p.second;
    writeStructMember(ss, c.type, c.name, VertexAttributeSemantics::Invalid,
                      false);
  }
  ss << "};\n";
}

inline std::string structName(const std::string &baseName) {
  return baseName + "Input";
}

std::string resolveInput(const MaterialNetwork *network,
                         const MaterialNode *node, const std::string &inputName,
                         const std::string &defaultResult) {
  struct {
    void operator()(const int &v) { result = std::to_string(v); }
    void operator()(const float &v) { result = std::to_string(v); }
    void operator()(const double &v) { result = std::to_string(v); }
    void operator()(const Vec2F &v) {
      result =
          "float2(" + std::to_string(X(v)) + ", " + std::to_string(Y(v)) + ")";
    }
    void operator()(const Vec3F &v) {
      result = "float3(" + std::to_string(X(v)) + ", " + std::to_string(Y(v)) +
               ", " + std::to_string(Z(v)) + ")";
    }
    void operator()(const Vec4F &v) {
      result = "float4(" + std::to_string(X(v)) + ", " + std::to_string(Y(v)) +
               ", " + std::to_string(Z(v)) + ", " + std::to_string(W(v)) + ")";
    }
    void operator()(const Mat2x2F &v) { result = ""; }
    void operator()(const Mat3x3F &v) { result = ""; }
    void operator()(const Mat4x4F &v) { result = ""; }
    void operator()(const std::string &v) { result = ""; }

    std::string result;
  } defaultValueVis;

  std::string output = defaultResult;
  if (auto input = node->GetInput(inputName)) {
    const auto &upstreamNodeName = input->upstreamNode;
    const auto &upstreamOutput = input->upstreamOutput;
    const auto &defaultValue = input->defaultValue;

    if (auto upstreamNode = network->GetMaterialNode(upstreamNodeName)) {
      return upstreamNodeName + "(in, uniforms)." + upstreamOutput;
    } else {
      std::visit(defaultValueVis, defaultValue);
      return defaultValueVis.result;
    }
  }
  return "";
}

using NodeVisitorImplementation =
    std::function<void(MaterialNetworkVisitor *, const std::string &,
                       const MaterialNode *, ShaderGenContext &)>;

void defaultVert(MaterialNetworkVisitor *vis, const std::string &outputName,
                 const MaterialNode *node, ShaderGenContext &genCtx) {
  std::stringstream &code = *genCtx.nodeCode;
  const auto &network = genCtx.materialNetwork;
  const std::string positionInput =
      resolveInput(network, node, "position", "float4(0,0,0,1)");
  const std::string projMatrix =
      resolveInput(network, node, "projectionMatrix", "float4x4(1)");
  const std::string modelMatrix =
      resolveInput(network, node, "modelMatrix", "float4x4(1)");
  const std::string viewMatrix =
      resolveInput(network, node, "viewMatrix", "float4x4(1)");
  code << "  " << outputName << " out;\n";
  code << "  float4x4 modelViewProjection = " + projMatrix + " * " +
              viewMatrix + " * " + modelMatrix + ";\n";
  code << "  out.position = modelViewProjection * " << positionInput << ";\n";
  code << "  return out;\n";
}

void defaultFrag(MaterialNetworkVisitor *vis, const std::string &outputName,
                 const MaterialNode *node, ShaderGenContext &genCtx) {
  std::stringstream &code = *genCtx.nodeCode;
  std::string colorInput =
      resolveInput(genCtx.materialNetwork, node, "color", "float4(1,0,0,1)");
  code << "  " << outputName << " out;\n";
  code << "  out.color = " << colorInput << ";\n";
  code << "  return out;\n";
}

void bufferView(MaterialNetworkVisitor *vis, const std::string &outputName,
                const MaterialNode *node, ShaderGenContext &genCtx) {
  auto bufferName = std::get_if<std::string>(node->GetParameter("bufferName"));
  auto semantics = std::get_if<int>(node->GetParameter("semantics"));
  auto type = std::get_if<int>(node->GetParameter("type"));

  if (bufferName == nullptr || semantics == nullptr || type == nullptr) {
    LOG_ERROR("Incomplete BufferView MaterialNode");
    return;
  }
  genCtx.bufferInputs.emplace_back(
      *bufferName, static_cast<VertexAttributeSemantics>(*semantics),
      static_cast<Type>(*type));
  genCtx.bufferInputsPerStage[genCtx.shaderStage].emplace_back(
      genCtx.bufferInputs.size() - 1);

  auto output = node->GetOutputs().begin();
  auto structOutName = output->second.name;

  std::stringstream &code = *genCtx.nodeCode;
  code << "  " << outputName << " out;\n";
  code << "  out." << structOutName << " = in." << *bufferName << ";\n";
  code << "  return out;\n";
}

void uniformView(MaterialNetworkVisitor *vis, const std::string &outputName,
                 const MaterialNode *node, ShaderGenContext &genCtx) {
  auto uniformName =
      std::get_if<std::string>(node->GetParameter("uniformName"));
  auto type = std::get_if<int>(node->GetParameter("type"));

  if (uniformName == nullptr || type == nullptr) {
    LOG_ERROR("Incomplete UniformView MaterialNode");
    return;
  }

  genCtx.uniformRequests.emplace_back(*uniformName, static_cast<Type>(*type));
  genCtx.uniformRequestsPerStage[genCtx.shaderStage].emplace_back(
      genCtx.uniformRequests.size() - 1);

  auto output = node->GetOutputs().begin();
  auto structOutName = output->second.name;

  *genCtx.nodeCode << "  " << outputName << " out;\n";
  *genCtx.nodeCode << "  out." << structOutName << " = uniforms->"
                   << *uniformName << ";\n";
  *genCtx.nodeCode << "  return out;\n";
}

// clang-format off
static const std::unordered_map<std::string, NodeVisitorImplementation> sNodeVisitors{
  {"defaultVert", defaultVert},
  {"defaultFrag", defaultFrag},
  {"bufferView", bufferView},
  {"uniformView", uniformView}
};
// clang-format on

} // namespace

ShaderGen::ShaderGen(MaterialNetwork &network) : m_network(network) {}

void ShaderGen::Visit(const MaterialNode *node) {
  const auto &nodeId = node->GetId();

  auto visIter = sNodeVisitors.find(nodeId);
  if (visIter == sNodeVisitors.end()) {
    LOG_ERROR("No node implementation for id '" << nodeId << "'");
    return;
  }

  auto codeIter = m_visitedNodes.emplace(node);
  if (!codeIter.second) {
    return;
  }

  // Visit all upstream nodes
  for (const auto &in : node->GetInputs()) {
    if (auto upstreamNode = m_network.GetMaterialNode(in.second.upstreamNode)) {
      Visit(upstreamNode);
    }
  }

  const auto &nodeName = node->GetName();
  const auto &shaderType = m_genCtx.shaderStage;
  const auto &outputName = structName(nodeName);

  DBG_ASSERT_MSG(!node->GetOutputs().empty(),
                 "A MaterialNode should always have outputs");
  m_shaderCode << "//// Begin " << shaderTypeName(shaderType)
               << " Vertex Shader Material Node '" << nodeName << "'\n";
  writeStruct(m_shaderCode, outputName, node->GetOutputs());

  m_shaderCode << outputName << " " << node->GetName() << "("
               << shaderTypeName(shaderType)
               << "In in, constant Uniforms* uniforms) {\n";
  visIter->second(this, outputName, node, m_genCtx);
  m_shaderCode << "}\n";

  m_shaderCode << "//// End " << shaderTypeName(shaderType)
               << " Vertex Shader Material Node '" << nodeName << "'\n";
}

std::string ShaderGen::Generate() {
  std::stringstream header;
  std::stringstream nodeCode;

  header << R"(#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
)";

  m_genCtx.nodeCode = &m_shaderCode;
  m_genCtx.materialNetwork = &m_network;

  m_genCtx.shaderStage = MaterialNetwork::ShaderStage::Vertex;
  auto vertTerminalNode =
      m_network.GetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex);
  Visit(vertTerminalNode);

  m_genCtx.shaderStage = MaterialNetwork::ShaderStage::Fragment;
  auto fragTerminalNode =
      m_network.GetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment);
  Visit(fragTerminalNode);

  const auto &fragTerminalNodeName = fragTerminalNode->GetName();
  const auto &vertTerminalNodeName = vertTerminalNode->GetName();

  std::stringstream vertex;

  const auto &vertexBufferInputs =
      m_genCtx.bufferInputsPerStage[MaterialNetwork::ShaderStage::Vertex];
  const auto &fragBufferInputs =
      m_genCtx.bufferInputsPerStage[MaterialNetwork::ShaderStage::Fragment];

  // We may need to emit the position to the frag shader.
  const BufferRequest *positionBuffer;

  // Create the Vertex Shader Input struct
  header << R"(
//// Vertex Shader Input
typedef struct
{
)";
  for (const auto &input : m_genCtx.bufferInputs) {
    header << "  " << typeName(input.type) << " " << input.name << ";\n";
    if (input.semantics == VertexAttributeSemantics::Position) {
      positionBuffer = &input;
    }
  }
  header << "} Vertex;\n";

  // Create the Fragment Shader Input struct
  header << R"(
//// Fragment Shader Input
typedef struct
{
)";
  bool emittedPosition = false;
  for (const auto &index : fragBufferInputs) {
    const auto &bufferInput = m_genCtx.bufferInputs[index];
    writeStructMember(header, bufferInput.type, bufferInput.name,
                      bufferInput.semantics, true);
    emittedPosition |=
        bufferInput.semantics == VertexAttributeSemantics::Position;
  }
  if (!emittedPosition) {
    writeStructMember(header, positionBuffer->type, positionBuffer->name,
                      positionBuffer->semantics, true);
  }
  header << R"(
} Fragment;
)";

  // Create the Uniforms struct
  header << R"(
//// Uniforms
typedef struct
{
)";
  for (const auto &uniform : m_genCtx.uniformRequests) {
    writeStructMember(header, uniform.type, uniform.name,
                      VertexAttributeSemantics::Invalid, false);
  }
  header << "} Uniforms;\n";

  // Create the Vertex Shader Material Node Input struct
  header << R"(
//// Vertex Material Node Input
typedef struct
{
)";
  for (const auto &index : vertexBufferInputs) {
    const auto &bufferInput = m_genCtx.bufferInputs[index];
    writeStructMember(header, bufferInput.type, bufferInput.name,
                      bufferInput.semantics, false);
  }
  header << "} VertexIn;\n";

  // Create the Fragment Shader Material Node Input struct
  header << R"(
//// Fragment Material Node Input
typedef struct
{
)";
  for (const auto &index : fragBufferInputs) {
    const auto &bufferInput = m_genCtx.bufferInputs[index];
    writeStructMember(header, bufferInput.type, bufferInput.name,
                      bufferInput.semantics, false);
  }
  header << "} FragmentIn;\n";

  // Write the Vertex Shader Entry point
  // clang-format off
	vertex << "vertex Fragment vertexShader(uint vertexID               [[ vertex_id ]],\n";
	vertex << "                             constant Vertex *vertices   [[ buffer(0) ]],\n";
	vertex << "                             constant Uniforms *uniforms [[ buffer(1) ]]) {\n";
  vertex << "  Vertex thisVertex = vertices[vertexID];\n";
  vertex << "  VertexIn vertexIn;\n";
  // clang-format on
  for (const auto &index : vertexBufferInputs) {
    const auto &bufferInput = m_genCtx.bufferInputs[index];
    vertex << "  vertexIn." << bufferInput.name << " = thisVertex."
           << bufferInput.name << ";\n";
  }
  vertex << "  " << structName(vertTerminalNode->GetName())
         << " terminal = " << vertTerminalNodeName << "(vertexIn, uniforms);\n";
  vertex << "  Fragment out;\n";
  for (const auto &index : fragBufferInputs) {
    const auto &bufferInput = m_genCtx.bufferInputs[index];
    if (auto terminalOutput = vertTerminalNode->GetOutput(bufferInput.name)) {
      vertex << "  out." << bufferInput.name << " = terminal."
             << bufferInput.name << ";\n";
    } else {
      vertex << "  out." << bufferInput.name << " = thisVertex."
             << bufferInput.name << ";\n";
    }
  }
  if (!emittedPosition) {
    const auto &positionBufferName = positionBuffer->name;
    if (auto terminalOutput = vertTerminalNode->GetOutput(positionBufferName)) {
      vertex << "  out." << positionBufferName << " = terminal."
             << positionBufferName << ";\n";
    } else {
      vertex << "  out." << positionBufferName << " = thisVertex."
             << positionBufferName << ";\n";
    }
  }
  vertex << "  return out;\n";
  vertex << "}\n";

  // Write the Fragment Shader Entry point
  std::stringstream fragment;
  fragment << "fragment float4 fragmentShader(Fragment thisFragment        [[ "
              "stage_in ]],\n";
  fragment << "                               constant Uniforms *uniforms  [[ "
              "buffer(0) ]]) {\n";
  fragment << "  FragmentIn in;\n";
  for (const auto &index : fragBufferInputs) {
    const auto &bufferInput = m_genCtx.bufferInputs[index];
    fragment << "  in." << bufferInput.name << " = thisFragment."
             << bufferInput.name << ";\n";
  }
  fragment << "  " << structName(fragTerminalNode->GetName())
           << " terminal = " << fragTerminalNodeName << "(in, uniforms);\n";
  fragment << "  return terminal.color;\n";
  fragment << "}\n";

  std::stringstream source;
  source << header.str() << "\n";
  source << m_shaderCode.str() << "\n";
  source << vertex.str() << "\n";
  source << fragment.str() << "\n";

  std::cout << "---------------------------------------------------------------"
               "-----------------"
            << std::endl;
  std::cout << source.str() << std::endl;
  std::cout << "---------------------------------------------------------------"
               "-----------------"
            << std::endl;

  return source.str();
}

const std::vector<BufferRequest> &ShaderGen::GetBufferRequests() const {
  return m_genCtx.bufferInputs;
}
const std::vector<UniformRequest> &ShaderGen::GetUniformRequests() const {
  return m_genCtx.uniformRequests;
}
} // namespace pgeditor::renderer::metal
