#pragma once

#include <memory>

namespace pagoda::material {
class MaterialNetwork;
using MaterialNodePtr = std::shared_ptr<class MaterialNode>;

class MaterialNetworkVisitor {
public:
  virtual void Visit(const MaterialNodePtr& node) = 0;
};
} // namespace pagoda::material
