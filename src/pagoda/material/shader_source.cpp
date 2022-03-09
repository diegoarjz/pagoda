#include "shader_source.h"

#include "material_node.h"

#include <memory>

namespace pagoda::material {
ShaderSource::~ShaderSource() {}
void ShaderSource::SetNode(const MaterialNodePtr &node) { m_node = node; }

const MaterialNodePtr &ShaderSource::GetNode() const { return m_node; }

const std::string &ShaderSource::NodeId() const { return m_node->GetId(); }

common::Type ShaderSource::InputType(const std::string &inputName) {
  if (auto input = m_node->GetInput(inputName)) {
    return input->type;
  }
  return common::Type::Invalid;
}

////////////////////////////////////////////////////////////
// MSL Shader Source
////////////////////////////////////////////////////////////
class MSLShaderSource : public ShaderSource {
public:
  MSLShaderSource(std::stringstream &s) : m_immediate{s} {}
  ~MSLShaderSource() override {}

  ShadingLanguage TargetLanguage() const override {
    return ShadingLanguage::MSL;
  }

  void EmitFunctionStart(const std::vector<std::string> &inputs) override {
    m_inputs = inputs;
  }

  void EmitLineImmediate(const std::string &l) override {
    m_immediate << l << "\n";
  }
  void EmitTextImmediate(const std::string &t) override { m_immediate << t; }

  void EmitLine(const std::string &l) override { m_body << l << "\n"; }
  void EmitText(const std::string &t) override { m_body << t; }

  void EmitFunctionEnd() override {
    const auto &node = GetNode();
    const auto &nodeHints = node->GetHints();
    const auto &outputs = node->GetOutputs();

    m_immediate << "struct " << NodeId() << "_output {\n";
    for (const auto &[_, output] : outputs) {
      const auto &outputHints = output.hints;
      auto stageOutputAttribute =
          outputHints.find(MaterialNode::HintKeys::stageOutAttribute);

      m_immediate << "  " << TypeName(output.type);

      if (stageOutputAttribute != outputHints.end()) {
        m_immediate << " " << output.name << " [[ "
                    << stageOutputAttribute->second << " ]];\n";
      } else {
        m_immediate << " " << output.name << ";\n";
      }
    }
    m_immediate << "};\n\n";

    auto stageOutputHint = nodeHints.find(MaterialNode::HintKeys::shaderStage);
    if (stageOutputHint != nodeHints.end()) {
      m_immediate << stageOutputHint->second << " ";
    }

    m_immediate << NodeId() << "_output " << NodeId() << "(";
    for (const auto &i : m_inputs) {
      const auto &nodeInput = node->GetInput(i);
      const auto inputType = nodeInput->type;
      const auto &inputHints = nodeInput->hints;

      auto typeHint = inputHints.find("type");
      if (inputType != common::Type::Invalid) {
        m_immediate << TypeName(inputType);
      } else if (typeHint != inputHints.end()) {
        m_immediate << typeHint->second;
      } else {
        std::cout << "No type defined" << std::endl;
      }
      m_immediate << " " << i;

      auto attributeHint = inputHints.find(MaterialNode::HintKeys::attribute);
      if (attributeHint != inputHints.end()) {
        m_immediate << " [[ " << attributeHint->second << " ]]";
      }

      if (i != m_inputs.back()) {
        m_immediate << ", ";
      }
    }
    m_immediate << ") {\n";
    m_immediate << m_body.str();
    m_immediate << "}\n\n";

    m_body = std::stringstream();
  }

  std::string TypeName(common::Type type) override {
    static const std::string sTypenames[] = {
        "int",    "int2",   "uint",   "uint2",    "float",    "double",
        "float2", "float3", "float4", "float2x2", "float3x3", "float4x4",
    };
    return sTypenames[static_cast<int>(type)];
  }

private:
  std::stringstream &m_immediate;
  std::stringstream m_body;
  std::vector<std::string> m_inputs;
};

std::shared_ptr<ShaderSource>
ShaderSource::Create(ShadingLanguage language, std::stringstream &outSource) {
  switch (language) {
  case ShadingLanguage::MSL:
    return std::make_shared<MSLShaderSource>(outSource);
  case ShadingLanguage::GLSL:
    return nullptr;
  }
}
} // namespace pagoda::material
