#pragma once

#include "material_node.h"
#include "types.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace pgeditor::renderer
{
class MaterialNetwork
{
	public:
	enum class ShaderStage
	{
		Vertex,
		Fragment
	};

	MaterialNetwork(const std::string& materialName);
	~MaterialNetwork();

	MaterialNode* CreateMaterialNode(const std::string& nodeId, const std::string& name);
	const MaterialNode* GetMaterialNode(const std::string& name) const;
	MaterialNode* GetMaterialNode(const std::string& name);

	void SetStageTerminalNode(ShaderStage stage, const std::string& name);
	MaterialNode* GetStageTerminalNode(ShaderStage stage);

	private:
	std::string m_materialName;
	std::array<std::string, 2> m_terminalNodes;
	std::unordered_map<std::string, MaterialNode> m_nodes;
};
}  // namespace pgeditor::renderer
