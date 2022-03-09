#pragma once

#include <pagoda/material/material_node.h>

namespace pagoda::material::nodes {

class BufferView : public MaterialNode {
public:
  BufferView(const std::string &suffix, common::Type type,
             common::Value defaultValue);

  const std::vector<std::string>& InputOrder() override;
  bool SourceCode(ShaderSource &source) override;

  void SetVertexAttributeName(const std::string& name);
};

} // namespace pagoda::material::nodes
