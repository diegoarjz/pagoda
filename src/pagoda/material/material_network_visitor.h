#pragma once

namespace pagoda::material {
class MaterialNetwork;
class MaterialNode;

class MaterialNetworkVisitor {
public:
  virtual void Visit(const MaterialNode *node) = 0;
};
} // namespace pagoda::material
