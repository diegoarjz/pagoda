#include "shader_gen.h"
#include "pagoda/material/material_node.h"

namespace pagoda::material {
////////////////////////////////////////
// ShaderStage
////////////////////////////////////////
ShaderStage::ShaderStage(Stage_t stageType, const MaterialNodePtr &node)
    : m_shaderStage{stageType}, m_materialNode{node} {

  std::unordered_set<MaterialNodeWeakPtr> visitedNodes;
  sortTopologically(m_materialNode, visitedNodes);

  populateRequests();
}

const ShaderStage::Stage_t &ShaderStage::GetStageType() const {
  return m_shaderStage;
}

const ShaderStage::StageInputRequestVector_t &
ShaderStage::GetStageInputRequests() const {
  return m_vertexAttributes;
}

const ShaderStage::UniformRequestVector_t &
ShaderStage::GetUniformRequests() const {
  return m_uniforms;
}

const ShaderStage::StageOutputVector_t &ShaderStage::GetStageOutputs() const {
  return m_stageOutputs;
}

std::string ShaderStage::GetSource(ShaderSource &source) {
  emitFunctions(source);
  emitMainFunction(source);
  return "";
}

void ShaderStage::populateRequests() {
  for (const auto &node : m_topologicalSortedNodes) {
    for (const auto &[_, input] : node->GetInputs()) {
      const auto &hints = input.hints;

      auto stageIn = hints.find(MaterialNode::HintKeys::stageIn);
      if (stageIn != hints.end()) {
        m_vertexAttributes.emplace_back(
            stageIn->second, VertexAttributeSemantics::Custom, input.type);
      }

      auto uniform = hints.find(MaterialNode::HintKeys::uniform);
      if (uniform != hints.end()) {
        m_uniforms.emplace_back(uniform->second, input.type);
      }
    }

    for (const auto &[_, output] : node->GetOutputs()) {
      const auto &hints = output.hints;
      auto stageOut = hints.find(MaterialNode::HintKeys::stageOut);
      if (stageOut != hints.end()) {
        m_stageOutputs.emplace_back(node, stageOut->second, output.type,
                                    output.hints);
      }
    }
  }
}

void ShaderStage::sortTopologically(
    const MaterialNodeWeakPtr &node,
    std::unordered_set<MaterialNodeWeakPtr> &visitedNodes) {
  if (visitedNodes.find(node) != visitedNodes.end()) {
    return;
  }

  node->Connections([this, &visitedNodes](const MaterialNode::Connection &c) {
    sortTopologically(c.upstreamNode, visitedNodes);
  });

  visitedNodes.insert(node);
  m_topologicalSortedNodes.push_back(node);
}

void ShaderStage::emitFunctions(ShaderSource &source) {
  std::unordered_set<std::string> emittedFunctions;
  for (const auto &node : m_topologicalSortedNodes) {
    if (emittedFunctions.find(node->GetId()) != emittedFunctions.end()) {
      // We have emitted a function for this shader type
      continue;
    }
    emittedFunctions.insert(node->GetId());

    source.SetNode(node);
    node->SourceCode(source);
  }
}

class MainFunction : public MaterialNode {
public:
  MainFunction(ShaderStage *stage,
               const std::vector<MaterialNodePtr> &sortedNodes)
      : MaterialNode(FunctionName(stage->GetStageType())), m_stage{stage},
        m_topologicalSortedNodes{sortedNodes} {}

  ~MainFunction() override {}

  const std::vector<std::string> &InputOrder() override { return m_inputOrder; }

  static const std::string &
  ShaderStageName(MaterialNetwork::ShaderStage stage) {
    static const std::string sNames[] = {"vertex", "fragment"};
    return sNames[static_cast<int>(stage)];
  }

  static std::string FunctionName(MaterialNetwork::ShaderStage stage) {
    return "main_" + ShaderStageName(stage);
  }

  bool SourceCode(ShaderSource &source) override {
    if (source.TargetLanguage() == ShaderSource::ShadingLanguage::MSL) {
      const auto &stageInputs = m_stage->GetStageInputRequests();
      const auto &uniforms = m_stage->GetUniformRequests();
      const auto &stageOutputs = m_stage->GetStageOutputs();
      uint32_t bufferCount = 0;

      std::string stageInputName;
      std::string stageInputStruct;
      std::string stageUniformStruct;
      if (m_stage->GetStageType() == MaterialNetwork::ShaderStage::Vertex) {
        // Set up the vertex shader data
        //
        stageInputName = "vertices";
        stageInputStruct = "Vertex";
        stageUniformStruct = "VertexUniforms";

        m_hints = {{HintKeys::shaderStage, "vertex"}};

        m_inputs = {{"vertexID",
                     {"vertexID",
                      Type_t::UInt,
                      0,
                      {{HintKeys::attribute, "vertex_id"}}}}};
        m_inputOrder = {"vertexID"};

        // For Metal we only emit stage in strcts for the vertex shader.
        // The other stages reuse the previous stage's output struct.
        if (!stageInputs.empty()) {
          MaterialNode::Input stageIn{
              stageInputName,
              Type_t::Invalid,
              0,
              {{HintKeys::type, "constant Vertex*"},
               {HintKeys::attribute,
                "buffer(" + std::to_string(bufferCount++) + ")"}}};

          m_inputs.emplace(stageInputName, stageIn);
          m_inputOrder.emplace_back("vertices");

          source.EmitLineImmediate("struct " + stageInputStruct + " {");
          for (const auto &attrib : stageInputs) {
            source.EmitLineImmediate("  " + source.TypeName(attrib.type) + " " +
                                     attrib.name + ";");
          }
          source.EmitLineImmediate("};\n");
        }

      } else {
        // Set up the fragment shader data
        //
        m_hints = {{HintKeys::shaderStage, "fragment"}};
        stageUniformStruct = "FragmentUniforms";

        stageInputName = "fragmentIn";
        if (!stageInputs.empty()) {
          // In a fragment shader we reuse the vertex shader's output struct
          MaterialNode::Input stageIn{stageInputName,
                                      Type_t::Invalid,
                                      0,
                                      {{HintKeys::type, "main_vertex_output"},
                                       {HintKeys::attribute, "stage_in"}}};
          m_inputs.emplace(stageInputName, stageIn);
        }

        m_inputOrder = {stageInputName};
      }

      if (!uniforms.empty()) {
        MaterialNode::Input uniform{
            "uniforms",
            Type_t::Invalid,
            0,
            {{HintKeys::type, "constant " + stageUniformStruct + "*"},
             {HintKeys::attribute,
              "buffer(" + std::to_string(bufferCount++) + ")"}}};

        m_inputs.emplace("uniforms", uniform);
        m_inputOrder.emplace_back("uniforms");

        source.EmitLineImmediate("struct  " + stageUniformStruct + " {");
        for (const auto &uniform : uniforms) {
          source.EmitLineImmediate("  " + source.TypeName(uniform.type) + " " +
                                   uniform.name + ";");
        }
        source.EmitLineImmediate("};\n");
      }

      for (const auto &stageOutput : stageOutputs) {
        MaterialNode::Output output{stageOutput.name, stageOutput.type,
                                    stageOutput.hints};
        m_outputs.emplace(stageOutput.name, output);
      }

      source.EmitFunctionStart(InputOrder());
      for (const auto &node : m_topologicalSortedNodes) {
        const auto &nodeInputOrder = node->InputOrder();
        const auto &nodeInputs = node->GetInputs();
        const auto &nodeConnections = node->GetConnections();

        source.EmitLine("  " + node->GetId() + "_output " + node->GetName() +
                        " = ");
        source.EmitLine("    " + node->GetId() + "(");

        for (const auto &inputName : nodeInputOrder) {
          const auto &input = nodeInputs.at(inputName);
          auto connection = nodeConnections.find(input.name);
          if (connection != nodeConnections.end()) {
            source.EmitText("      " +
                            connection->second.upstreamNode->GetName() + "." +
                            connection->second.upstreamOutput);
          } else {
            const auto &hints = input.hints;
            auto stageIn = hints.find(MaterialNode::HintKeys::stageIn);
            auto uniform = hints.find(MaterialNode::HintKeys::uniform);
            if (stageIn != hints.end()) {
              auto vertexAttribute = hints.find("vertex-attribute");
              // TODO: Checking the stage type here should be avoided. Maybe
              // there should be a hint saying whether the input is a pointer
              // or not
              if (m_stage->GetStageType() ==
                  MaterialNetwork::ShaderStage::Vertex) {
                if (vertexAttribute != hints.end()) {
                  source.EmitText("      " + stageInputName + "[vertexID]." +
                                  vertexAttribute->second);
                } else {
                  source.EmitText("      " + stageInputName + "[vertexID]." +
                                  input.name);
                }
              } else {
                source.EmitText("      " + stageInputName + "." + input.name);
              }
            } else if (uniform != hints.end()) {
              source.EmitText("      uniforms->" + uniform->second);
            }
          }

          if (inputName != nodeInputOrder.back()) {
            source.EmitText(",\n");
          }
        }
        source.EmitLine(");");
      }

      source.EmitLine("  " + GetId() + "_output result;");
      for (const auto &stageOutput : stageOutputs) {
        source.EmitLine("  result." + stageOutput.name + " = " +
                        stageOutput.sourceNode->GetName() + "." +
                        stageOutput.name + ";");
      }
      source.EmitLine("  return result;");

      source.EmitFunctionEnd();
    }
    return false;
  }

private:
  ShaderStage *m_stage;
  const std::vector<MaterialNodePtr> &m_topologicalSortedNodes;
  std::vector<std::string> m_inputOrder;
};

void ShaderStage::emitMainFunction(ShaderSource &source) {
  auto mainFunction =
      std::make_shared<MainFunction>(this, m_topologicalSortedNodes);
  source.SetNode(mainFunction);
  mainFunction->SourceCode(source);
}

////////////////////////////////////////
// ShaderGen
////////////////////////////////////////
ShaderGen::ShaderGen(MaterialNetwork &network) : m_materialNetwork{network} {
  // Create a Shader Stage for all terminals
  for (auto stage : {ShaderStage_t::Fragment, ShaderStage_t::Vertex}) {
    if (auto terminal = m_materialNetwork.GetStageTerminalNode(stage)) {
      m_shaderStages.emplace(stage, ShaderStage(stage, terminal));
    }
  }
}

ShaderGen::~ShaderGen() {}

ShaderStage *ShaderGen::GetStage(ShaderStage_t stageType) {
  auto iter = m_shaderStages.find(stageType);
  if (iter == m_shaderStages.end()) {
    return nullptr;
  }
  return &iter->second;
}
} // namespace pagoda::material
