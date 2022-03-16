#include "material_system.h"

#include "material_component.h"

#include "operations/default_material.h"
#include "operations/set_material.h"
#include "operations/set_material_attribute.h"
#include "operations/set_shader.h"
#include "operations/set_texture.h"

#include "nodes/default_frag.h"
#include "nodes/default_vert.h"
#include "nodes/buffer_view.h"

#include <memory>
#include <pagoda/pagoda.h>

namespace pagoda::material {
using namespace operations;

const std::string MaterialSystem::GetComponentSystemName() {
  return "MaterialSystem";
}

MaterialSystem::MaterialSystem()
    : ProceduralComponentSystem(GetComponentSystemName()) {}

MaterialSystem::~MaterialSystem() {}

void MaterialSystem::SetNamedMaterial(const std::string &name,
                                      const Material &material) {
  m_namedMaterials.emplace(name, material);
}
const Material &MaterialSystem::GetNamedMaterial(const std::string &name) {
  return m_namedMaterials.at(name);
}

MaterialNodeRegistry *MaterialSystem::GetMaterialNodeRegistry() {
  if (m_materialNodeRegistry == nullptr) {
    m_materialNodeRegistry = std::make_unique<MaterialNodeRegistry>();
  }
  return m_materialNodeRegistry.get();
}

void MaterialSystem::DoClone(std::shared_ptr<MaterialComponent> from,
                             std::shared_ptr<MaterialComponent> to) {
  to->GetMaterial() = from->GetMaterial();
}

namespace {
template <class MaterialNodeType>
void registerMaterialNode(MaterialNodeRegistry *reg) {
  reg->Register(MaterialNodeType::nodeName,
                []() { return std::make_shared<MaterialNodeType>(); });
}
} // namespace

void MaterialSystem::Registration(Pagoda *pagoda) {
  LOG_TRACE(Core, "Registering " << GetComponentSystemName());

  auto materialSystem = std::make_shared<MaterialSystem>();
  auto materialNodeRegistry = materialSystem->GetMaterialNodeRegistry();

  // Material Nodes
  registerMaterialNode<nodes::DefaultFrag>(materialNodeRegistry);
  registerMaterialNode<nodes::DefaultVert>(materialNodeRegistry);
  materialNodeRegistry->Register("buffer_view_vec3", []() {
    return std::make_shared<nodes::BufferView>("vec3", common::Type::Vec3, math::Vec3F{0,0,0});
  });
  materialNodeRegistry->Register("buffer_view_vec4", []() {
    return std::make_shared<nodes::BufferView>("vec4", common::Type::Vec4, math::Vec4F{0,0,0,0});
  });


  auto operationFactory = pagoda->GetOperationFactory();
  auto objectSystem = pagoda->GetProceduralObjectSystem();

  objectSystem->RegisterProceduralComponentSystem(materialSystem);

  // Operations
  operationFactory->Register("SetMaterial", [objectSystem]() {
    return std::make_shared<SetMaterial>(objectSystem);
  });
  operationFactory->Register("DefaultMaterial", [objectSystem]() {
    return std::make_shared<DefaultMaterial>(objectSystem);
  });
  /*
  operationFactory->Register("SetTexture", [objectSystem]() {
    return std::make_shared<SetTexture>(objectSystem);
  });
  operationFactory->Register("SetShader", [objectSystem]() {
    return std::make_shared<SetShader>(objectSystem);
  });
  operationFactory->Register("SetMaterialAttribute", [objectSystem]() {
    return std::make_shared<SetMaterialAttribute>(objectSystem);
  });
  */
}

} // namespace pagoda::material
