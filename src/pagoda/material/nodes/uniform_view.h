#pragma once

#include <pagoda/material/material_node.h>

namespace pagoda::material::nodes {

class UniformView : public MaterialNode {
public:
  UniformView(const std::string &suffix, common::Type type,
             common::Value defaultValue);

  const std::vector<std::string>& InputOrder() override;
  bool SourceCode(ShaderSource &source) override;

  void SetUniformName(const std::string& name);
};

} // namespace pagoda::material::nodes

