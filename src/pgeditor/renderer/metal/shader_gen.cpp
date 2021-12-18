#include "shader_gen.h"

#include <pagoda/common/debug/logger.h>

#include <pgeditor/renderer/material_network.h>
#include <pgeditor/renderer/material_node.h>

#include <iostream>
#include <stack>

namespace pgeditor::renderer::metal
{
namespace
{
const std::string& typeName(Type t)
{
  static const std::string sTypenames[] = {
	  "int",
	  "int2",
	  "uint",
	  "uint2",
	  "float",
	  "double",
	  "float2",
	  "float3",
	  "float4",
	  "float2x2",
	  "float3x3",
	  "float4x4",
  };
  return sTypenames[static_cast<int>(t)];
}

template<class T>
void writeStruct(std::stringstream& ss, const std::string& name, const T& contents)
{
	ss << "struct " << name << " {\n";
	for (const auto& p : contents) {
		auto c = p.second;
		ss << "  " << typeName(c.m_type) << " " << c.m_name << ";\n";
	}
	ss << "};\n";
}

inline std::string structName(const std::string& baseName, bool in) { return baseName + (in ? "Input" : "Output"); }

using NodeVisitorImplementation =
  std::function<void(MaterialNetworkVisitor*, const std::string&, const MaterialNode*, ShaderGenContext&)>;

void defaultVert(MaterialNetworkVisitor* vis, const std::string& outputName, const MaterialNode* node,
                 ShaderGenContext& genCtx)
{
	std::stringstream& code = *genCtx.nodeCode;

	std::string positionInput = "float4(0,0,0,1)";
	if (const auto& input = node->GetInput("position")) {
		const auto& upstreamNodeName = input->m_upstreamNode;
		const auto& upstreamOutput = input->m_upstreamOutput;

		if (auto upstreamNode = genCtx.materialNetwork->GetMaterialNode(upstreamNodeName)) {
			positionInput = upstreamNodeName + "(in, uniforms)." + upstreamOutput;
		}
	}

	code << "  " << outputName << " out;\n";
	code << "  out.position = " << positionInput << ";\n";
	code << "  return out;\n";
}

void defaultFrag(MaterialNetworkVisitor* vis, const std::string& outputName, const MaterialNode* node,
                 ShaderGenContext& genCtx)
{
	std::string colorInput = "float4(1,0,0,1)";
	if (auto input = node->GetInput("color")) {
		const auto& upstreamNodeName = input->m_upstreamNode;
		const auto& upstreamOutput = input->m_upstreamOutput;

		if (auto upstreamNode = genCtx.materialNetwork->GetMaterialNode(upstreamNodeName)) {
			colorInput = upstreamNodeName + "(in, uniforms)." + upstreamOutput;
		}
	}

	*genCtx.nodeCode << "  " << outputName << " out;\n";
	*genCtx.nodeCode << "  out.color = " << colorInput << ";\n";
	*genCtx.nodeCode << "  return out;\n";
}

void bufferView(MaterialNetworkVisitor* vis, const std::string& outputName, const MaterialNode* node,
                ShaderGenContext& genCtx)
{
	auto bufferName = std::get_if<std::string>(node->GetParameter("bufferName"));
	auto semantics = std::get_if<int>(node->GetParameter("semantics"));
	auto type = std::get_if<int>(node->GetParameter("type"));

	if (bufferName == nullptr || semantics == nullptr || type == nullptr) {
		LOG_ERROR("Incomplete BufferView MaterialNode");
		return;
	}
	genCtx.bufferInputs.emplace_back(*bufferName, static_cast<VertexAttributeSemantics>(*semantics),
	                                 static_cast<Type>(*type));
	genCtx.bufferInputsPerStage[genCtx.shaderStage].emplace_back(genCtx.bufferInputs.size() - 1);

	auto output = node->GetOutputs().begin();
	auto structOutName = output->second.m_name;

	*genCtx.nodeCode << "  " << outputName << " out;\n";
	*genCtx.nodeCode << "  out." << structOutName << " = in." << *bufferName << ";\n";
	*genCtx.nodeCode << "  return out;\n";
}

void uniformView(MaterialNetworkVisitor* vis, const std::string& outputName, const MaterialNode* node,
                 ShaderGenContext& genCtx)
{
	auto uniformName = std::get_if<std::string>(node->GetParameter("uniformName"));
	auto type = std::get_if<int>(node->GetParameter("type"));

	if (uniformName == nullptr || type == nullptr) {
		LOG_ERROR("Incomplete UniformView MaterialNode");
		return;
	}

	genCtx.uniformRequests.emplace_back(*uniformName, static_cast<Type>(*type));
	genCtx.uniformRequestsPerStage[genCtx.shaderStage].emplace_back(genCtx.uniformRequests.size() - 1);

	auto output = node->GetOutputs().begin();
	auto structOutName = output->second.m_name;

	*genCtx.nodeCode << "  " << outputName << " out;\n";
	*genCtx.nodeCode << "  out." << structOutName << " = uniforms->" << *uniformName << ";\n";
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

}  // namespace

ShaderGen::ShaderGen(MaterialNetwork& network) : m_network(network) {}

void ShaderGen::Visit(const MaterialNode* node)
{
	const auto& nodeId = node->GetId();

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
	for (const auto& in : node->GetInputs()) {
		if (auto upstreamNode = m_network.GetMaterialNode(in.second.m_upstreamNode)) {
			Visit(upstreamNode);
		}
	}

	const auto& nodeName = node->GetName();
	const auto& outputName = structName(nodeName, false);

	DBG_ASSERT_MSG(!node->GetOutputs().empty(), "A MaterialNode should always have outputs");
	if (m_genCtx.shaderStage == MaterialNetwork::ShaderStage::Vertex) {
		m_shaderCode << "//// Begin Vertex Shader Material Node '" << nodeName << "'\n";
	} else {
		m_shaderCode << "//// Begin Fragment Shader Material Node '" << nodeName << "'\n";
	}
	writeStruct(m_shaderCode, outputName, node->GetOutputs());

	if (m_genCtx.shaderStage == MaterialNetwork::ShaderStage::Vertex) {
		m_shaderCode << outputName << " " << node->GetName() << "(VertexIn in, constant Uniforms* uniforms) {\n";
	} else {
		m_shaderCode << outputName << " " << node->GetName() << "(FragmentIn in, constant Uniforms* uniforms) {\n";
	}
	visIter->second(this, outputName, node, m_genCtx);
	m_shaderCode << "}\n";
	if (m_genCtx.shaderStage == MaterialNetwork::ShaderStage::Vertex) {
		m_shaderCode << "//// End Vertex Shader Material Node '" << nodeName << "'\n\n";
	} else {
		m_shaderCode << "//// End Fragment Shader Material Node '" << nodeName << "'\n\n";
	}
}

std::string ShaderGen::Generate()
{
	std::stringstream header;
	std::stringstream nodeCode;

	header << R"(#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
)";

	m_genCtx.nodeCode = &m_shaderCode;
	m_genCtx.materialNetwork = &m_network;

	m_genCtx.shaderStage = MaterialNetwork::ShaderStage::Vertex;
	auto vertTerminalNode = m_network.GetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex);
	Visit(vertTerminalNode);

	m_genCtx.shaderStage = MaterialNetwork::ShaderStage::Fragment;
	auto fragTerminalNode = m_network.GetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment);
	Visit(fragTerminalNode);

	const auto& fragTerminalNodeName = fragTerminalNode->GetName();
	const auto& vertTerminalNodeName = vertTerminalNode->GetName();

	std::stringstream vertex;

	const auto& vertexBufferInputs = m_genCtx.bufferInputsPerStage[MaterialNetwork::ShaderStage::Vertex];
	const auto& fragBufferInputs = m_genCtx.bufferInputsPerStage[MaterialNetwork::ShaderStage::Fragment];

	// We may need to emit the position to the frag shader.
	const BufferRequest* positionBuffer;

	// Create the Vertex Shader Input struct
	header << R"(
//// Vertex Shader Input
typedef struct
{
)";
	for (const auto& input : m_genCtx.bufferInputs) {
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
	for (const auto& index : fragBufferInputs) {
		const auto& bufferInput = m_genCtx.bufferInputs[index];
		header << "  " << typeName(bufferInput.type);
		if (bufferInput.semantics == VertexAttributeSemantics::Position) {
			header << " " << bufferInput.name << " [[ position ]];\n";
			emittedPosition = true;
		} else {
			header << " " << bufferInput.name << ";\n";
		}
	}
	if (!emittedPosition) {
		header << "  " << typeName(positionBuffer->type) << " " << positionBuffer->name << " [[ position ]];\n";
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
	for (const auto& uniform : m_genCtx.uniformRequests) {
		header << "  " << typeName(uniform.type) << " " << uniform.name << ";\n";
	}
	header << "} Uniforms;\n";

	// Create the Vertex Shader Material Node Input struct
	header << R"(
//// Vertex Material Node Input
typedef struct
{
)";
	for (const auto& index : vertexBufferInputs) {
		const auto& bufferInput = m_genCtx.bufferInputs[index];
		header << "  " << typeName(bufferInput.type) << " " << bufferInput.name << ";\n";
	}
	header << "} VertexIn;\n";

	// Create the Fragment Shader Material Node Input struct
	header << R"(
//// Fragment Material Node Input
typedef struct
{
)";
	for (const auto& index : fragBufferInputs) {
		const auto& bufferInput = m_genCtx.bufferInputs[index];
		header << "  " << typeName(bufferInput.type) << " " << bufferInput.name << ";\n";
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
	for (const auto& index : vertexBufferInputs) {
		const auto& bufferInput = m_genCtx.bufferInputs[index];
		vertex << "  vertexIn." << bufferInput.name << " = thisVertex." << bufferInput.name << ";\n";
	}
	vertex << "  " << structName(vertTerminalNode->GetName(), false) << " terminal = " << vertTerminalNodeName
	       << "(vertexIn, uniforms);\n";
	vertex << "  Fragment out;\n";
	for (const auto& index : fragBufferInputs) {
		const auto& bufferInput = m_genCtx.bufferInputs[index];
		if (auto terminalOutput = vertTerminalNode->GetOutput(bufferInput.name)) {
			vertex << "  out." << bufferInput.name << " = terminal." << bufferInput.name << ";\n";
		} else {
			vertex << "  out." << bufferInput.name << " = thisVertex." << bufferInput.name << ";\n";
		}
	}
	if (!emittedPosition) {
		const auto& positionBufferName = positionBuffer->name;
		if (auto terminalOutput = vertTerminalNode->GetOutput(positionBufferName)) {
			vertex << "  out." << positionBufferName << " = terminal." << positionBufferName << ";\n";
		} else {
			vertex << "  out." << positionBufferName << " = thisVertex." << positionBufferName << ";\n";
		}
	}
	vertex << "  return out;\n";
	vertex << "}\n";

	// Write the Fragment Shader Entry point
	std::stringstream fragment;
	fragment << "fragment float4 fragmentShader(Fragment thisFragment [[stage_in]],\n";
	fragment << "                               constant Uniforms *uniforms) {\n";
	fragment << "  FragmentIn in;\n";
	for (const auto& index : fragBufferInputs) {
		const auto& bufferInput = m_genCtx.bufferInputs[index];
		fragment << "  in." << bufferInput.name << " = thisFragment." << bufferInput.name << ";\n";
	}
	fragment << "  " << structName(fragTerminalNode->GetName(), false) << " terminal = " << fragTerminalNodeName
	         << "(in, uniforms);\n";
	fragment << "  return terminal.color;\n";
	fragment << "}\n";

	std::stringstream source;
	source << header.str() << "\n";
	source << m_shaderCode.str() << "\n";
	source << vertex.str() << "\n";
	source << fragment.str() << "\n";

	return source.str();
}

const std::vector<BufferRequest>& ShaderGen::GetBufferRequests() const { return m_genCtx.bufferInputs; }
const std::vector<UniformRequest>& ShaderGen::GetUniformRequests() const { return m_genCtx.uniformRequests; }
}  // namespace pgeditor::renderer::metal
