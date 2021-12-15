#pragma once

#include "render_pipeline_state.h"

#include <pgeditor/renderer/material_network.h>
#include <pgeditor/renderer/material_network_visitor.h>
#include <pgeditor/renderer/vertex_attribute.h>

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace pgeditor::renderer
{
class MaterialNetwork;
class MaterialNode;
}  // namespace pgeditor::renderer

namespace pgeditor::renderer::metal
{
struct BufferRequest
{
	BufferRequest(const std::string& n, VertexAttributeSemantics s, Type t) : name{n}, semantics{s}, type{t} {}

	std::string name;
	VertexAttributeSemantics semantics;
	Type type;
};

struct UniformRequest
{
	UniformRequest(const std::string& n, Type t) : name{n}, type{t} {}

	std::string name;
	Type type;
};

struct ShaderGenContext
{
	const MaterialNetwork* materialNetwork;
	std::stringstream* nodeCode;
	std::vector<BufferRequest> bufferInputs;
	std::unordered_map<MaterialNetwork::ShaderStage, std::vector<std::size_t>> bufferInputsPerStage;
	std::vector<UniformRequest> uniformRequests;
	std::unordered_map<MaterialNetwork::ShaderStage, std::vector<std::size_t>> uniformRequestsPerStage;
	MaterialNetwork::ShaderStage shaderStage;
};

class ShaderGen : public MaterialNetworkVisitor
{
	public:
	ShaderGen(MaterialNetwork& network);

	std::string Generate();
	const std::vector<BufferRequest>& GetBufferRequests() const;
	const std::vector<UniformRequest>& GetUniformRequests() const;

	private:
	void Visit(const MaterialNode* node) override;

	std::unordered_set<const MaterialNode*> m_visitedNodes;
	std::stringstream m_shaderCode;

	ShaderGenContext m_genCtx;
	MaterialNetwork& m_network;
};
}  // namespace pgeditor::renderer::metal
