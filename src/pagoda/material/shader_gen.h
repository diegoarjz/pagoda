#pragma once

#include "material_network.h"
#include "material_network_visitor.h"

namespace pagoda::material {
struct StageInputRequest {
  StageInputRequest(const std::string &n, VertexAttributeSemantics s,
                         common::Type t)
      : name{n}, semantics{s}, type{t} {}

  std::string name;
  VertexAttributeSemantics semantics;
  common::Type type;
};

struct UniformRequest {
  UniformRequest(const std::string &n, common::Type t) : name{n}, type{t} {}

  std::string name;
  common::Type type;
};

struct StageOutput {
  StageOutput(const MaterialNodePtr &sourceNode, const std::string &n,
              common::Type t, const MaterialNode::Hints_t &hints)
      : sourceNode{sourceNode}, name{n}, type{t}, hints{hints} {}

  MaterialNodePtr sourceNode;
  std::string name;
  common::Type type;
  MaterialNode::Hints_t hints;
};

class ShaderStage {
public:
  using Stage_t = MaterialNetwork::ShaderStage;
  using StageInputRequestVector_t = std::vector<StageInputRequest>;
  using UniformRequestVector_t = std::vector<UniformRequest>;
  using StageOutputVector_t = std::vector<StageOutput>;

  ShaderStage(Stage_t stageType, const MaterialNodePtr &terminal);

  const Stage_t &GetStageType() const;

  const StageInputRequestVector_t &GetStageInputRequests() const;
  const UniformRequestVector_t &GetUniformRequests() const;
  const StageOutputVector_t &GetStageOutputs() const;

  std::string GetSource(ShaderSource &source);

private:
  void sortTopologically(const MaterialNodeWeakPtr &node,
                         std::unordered_set<MaterialNodeWeakPtr> &visitedNodes);
  void populateRequests();
  void emitFunctions(ShaderSource &source);
  void emitMainFunction(ShaderSource &source);

  Stage_t m_shaderStage;
  MaterialNodePtr m_materialNode;
  std::vector<StageInputRequest> m_vertexAttributes;
  std::vector<UniformRequest> m_uniforms;
  std::vector<StageOutput> m_stageOutputs;
  std::vector<MaterialNodePtr> m_topologicalSortedNodes;
};

class ShaderGen {
public:
  using ShaderStage_t = MaterialNetwork::ShaderStage;
  using ShaderStageMap_t = std::unordered_map<ShaderStage_t, ShaderStage>;

  ShaderGen(MaterialNetwork &network);
  virtual ~ShaderGen();

  ShaderStage *GetStage(ShaderStage_t stageType);

private:
  MaterialNetwork &m_materialNetwork;
  ShaderStageMap_t m_shaderStages;
};
} // namespace pagoda::material
