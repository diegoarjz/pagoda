#pragma once

#include "render_pipeline_state.h"

#include <pgeditor/renderer/vertex_attribute.h>

#include <pagoda/material/material_network.h>
#include <pagoda/material/material_network_visitor.h>

#include <pagoda/common/types.h>

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace pagoda::material
{
class MaterialNetwork;
class MaterialNode;
}  // namespace pgeditor::renderer

namespace pgeditor::renderer::metal
{
struct BufferRequest
{
	BufferRequest(const std::string& n, VertexAttributeSemantics s, pagoda::common::Type t) : name{n}, semantics{s}, type{t} {}

	std::string name;
	VertexAttributeSemantics semantics;
  pagoda::common::Type type;
};

struct UniformRequest
{
	UniformRequest(const std::string& n, pagoda::common::Type t) : name{n}, type{t} {}

	std::string name;
  pagoda::common::Type type;
};

struct ShaderGenContext
{
	const pagoda::material::MaterialNetwork* materialNetwork;
	std::stringstream* nodeCode;
	std::vector<BufferRequest> bufferInputs;
	std::unordered_map<pagoda::material::MaterialNetwork::ShaderStage, std::vector<std::size_t>> bufferInputsPerStage;
	std::vector<UniformRequest> uniformRequests;
	std::unordered_map<pagoda::material::MaterialNetwork::ShaderStage, std::vector<std::size_t>> uniformRequestsPerStage;
  pagoda::material::MaterialNetwork::ShaderStage shaderStage;
};

class ShaderGen : public pagoda::material::MaterialNetworkVisitor
{
	public:
	ShaderGen(pagoda::material::MaterialNetwork& network);

	std::string Generate();
	const std::vector<BufferRequest>& GetBufferRequests() const;
	const std::vector<UniformRequest>& GetUniformRequests() const;

	private:
	void Visit(const pagoda::material::MaterialNode* node) override;

	std::unordered_set<const pagoda::material::MaterialNode*> m_visitedNodes;
	std::stringstream m_shaderCode;

	ShaderGenContext m_genCtx;
  pagoda::material::MaterialNetwork& m_network;
};
}  // namespace pgeditor::renderer::metal
