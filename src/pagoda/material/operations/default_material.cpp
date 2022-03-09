#include "default_material.h"

#include <pagoda/material/nodes/default_frag.h>
#include <pagoda/material/nodes/default_vert.h>

#include <pagoda/material/material_network.h>
#include <pagoda/material/material_node.h>
#include <pagoda/material/material_system.h>

#include <pagoda/objects/interface.h>
#include <pagoda/objects/interface_callback.h>

#include <pagoda/common/types.h>

using namespace pagoda::common;
using namespace pagoda::objects;
using namespace pagoda::material::nodes;

namespace pagoda::material::operations {

DefaultMaterial::DefaultMaterial(ProceduralObjectSystemPtr objectSystem)
    : ProceduralOperation(objectSystem) {}

const std::string &DefaultMaterial::GetOperationName() const {
  static const std::string &sName{"DefaultMaterial"};
  return sName;
}

void DefaultMaterial::Interfaces(objects::InterfaceCallback *cb) {
  cb->OutputInterface(m_output, "out", "Out", Interface::Arity::One);
}

void DefaultMaterial::DoWork() {

  auto proceduralObject = CreateOutputProceduralObject();
  auto materialSystem =
      m_proceduralObjectSystem->GetComponentSystem<MaterialSystem>();
  auto materialComponent =
      materialSystem->CreateComponentAs<MaterialComponent>(proceduralObject);

  auto materialNetwork = std::make_shared<MaterialNetwork>(
      materialSystem->GetMaterialNodeRegistry(), "default");

  const std::string vertShaderName = "vertexShader";
  const std::string fragShaderName = "fragmentShader";
  materialNetwork->CreateMaterialNode(DefaultVert::nodeName, vertShaderName);
  materialNetwork->CreateMaterialNode(DefaultFrag::nodeName, fragShaderName);
  materialNetwork->SetStageTerminalNode(MaterialNetwork::ShaderStage::Vertex,
                                        vertShaderName);
  materialNetwork->SetStageTerminalNode(MaterialNetwork::ShaderStage::Fragment,
                                        fragShaderName);

  materialComponent->SetMaterial(materialNetwork);

  m_output->Set(proceduralObject);
}

} // namespace pagoda::material::operations
