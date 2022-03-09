#pragma once

#include "material_node.h"

#include <pagoda/common/factory.h>

namespace pagoda::material {
class MaterialNodeRegistry
    : public pagoda::common::Factory<MaterialNode, std::string> {
public:
  MaterialNodeRegistry();
  ~MaterialNodeRegistry() override;
};
} // namespace pagoda::material
