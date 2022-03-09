#pragma once

#include <pagoda/material/material_node.h>

namespace pagoda::material::nodes {
class DefaultVert : public MaterialNode {
public:
  constexpr static const char* const nodeName = "default_vert";

  DefaultVert();
  ~DefaultVert() override;

  const std::vector<std::string>& InputOrder() override;
  bool SourceCode(ShaderSource &source) override;
};
} // namespace pagoda::material::nodes
