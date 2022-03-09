#pragma once

#include "material_node.h"

#include <pagoda/common/types.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace pagoda::material {
class MaterialNodeRegistry;

// TODO: This was copied from pgeditor/renderer/vertex_attribute
//       and should be defined at a material level and not at a renderer level
enum class VertexAttributeSemantics {
  Position,
  Normal,
  TexCoord,
  Color,
  Custom,
  Invalid
};


using MaterialNodeMap = std::unordered_map<std::string, MaterialNodePtr>;

class MaterialNetwork {
public:
  enum class ShaderStage { Vertex, Fragment };

  ////////////////////////////////////////
  /// \n name Constructors
  ////////////////////////////////////////
  MaterialNetwork(MaterialNodeRegistry *nodeRegistry,
                  const std::string &materialName);
  ~MaterialNetwork();

  ////////////////////////////////////////
  /// \n name Getters & Setters
  ////////////////////////////////////////
  const std::string &GetName() const;

  ////////////////////////////////////////
  /// \n name Material Nodes
  ////////////////////////////////////////
  MaterialNodePtr CreateMaterialNode(const std::string &nodeId,
                                     const std::string &name);
  const MaterialNodePtr GetMaterialNode(const std::string &name) const;
  MaterialNodePtr GetMaterialNode(const std::string &name);

  ////////////////////////////////////////
  /// \n name Terminal Nodes
  ////////////////////////////////////////
  void SetStageTerminalNode(ShaderStage stage, const std::string &name);
  MaterialNodePtr GetStageTerminalNode(ShaderStage stage);

  ////////////////////////////////////////
  /// \name Hash
  ////////////////////////////////////////
  void AppendHash(std::size_t &hash) const;

private:
  MaterialNodeRegistry* m_nodeRegistry;
  std::string m_materialName;
  std::array<std::string, 2> m_terminalNodes;
  MaterialNodeMap m_nodes;
};
} // namespace pagoda::material
