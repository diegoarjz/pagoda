#pragma once

#include <pagoda/material/material_node.h>

namespace pagoda::material::nodes {
class DefaultFrag : public MaterialNode {
public:
  constexpr static const char* const nodeName = "default_frag";

  DefaultFrag();
  ~DefaultFrag() override;

  const std::vector<std::string>& InputOrder() override;
  bool SourceCode(ShaderSource &source) override;
};
}
