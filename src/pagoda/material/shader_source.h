#pragma once

#include <pagoda/common/types.h>

#include <string>

namespace pagoda::material {
using MaterialNodePtr = std::shared_ptr<class MaterialNode>;
using MaterialNodeWeakPtr = std::shared_ptr<class MaterialNode>;

class ShaderSource {
public:
  enum class ShadingLanguage { GLSL, MSL };

  virtual ~ShaderSource();

  virtual ShadingLanguage TargetLanguage() const = 0;

  virtual void EmitLineImmediate(const std::string &) = 0;
  virtual void EmitTextImmediate(const std::string &) = 0;

  virtual void EmitFunctionStart(const std::vector<std::string> &) = 0;
  virtual void EmitLine(const std::string &) = 0;
  virtual void EmitText(const std::string &) = 0;
  virtual void EmitFunctionEnd() = 0;
  virtual std::string TypeName(common::Type type) = 0;

  void SetNode(const MaterialNodePtr &node);
  const MaterialNodePtr &GetNode() const;
  const std::string &NodeId() const;
  common::Type InputType(const std::string &input);

  static std::shared_ptr<ShaderSource> Create(ShadingLanguage language, std::stringstream& outSource);
private:
  MaterialNodePtr m_node;
};

} // namespace pagoda::material
